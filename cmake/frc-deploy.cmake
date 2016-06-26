string(REGEX MATCH "^[0-9]+$" TEAM_NUMBER "${TEAM_NUMBER}")
if("${TEAM_NUMBER}" STREQUAL "")
    message(FATAL_ERROR "missing/invalid TEAM_NUMBER")
endif()
if("${ROBOT_EXECUTABLE}" STREQUAL "")
    message(FATAL_ERROR "missing ROBOT_EXECUTABLE")
endif()
if("${SSH_EXECUTABLE}" STREQUAL "")
    message(FATAL_ERROR "missing SSH_EXECUTABLE")
endif()
if("${SCP_EXECUTABLE}" STREQUAL "")
    message(FATAL_ERROR "missing SCP_EXECUTABLE")
endif()
if("${WPILIB_HOME}" STREQUAL "")
    message(FATAL_ERROR "missing WPILIB_HOME")
endif()

math(EXPR TEAM_NUMBER_HIGH "${TEAM_NUMBER} / 100")
math(EXPR TEAM_NUMBER_LOW "${TEAM_NUMBER} % 100")

# Use FRC_PREFER_TARGET to try a specific target first
# Can be "mdns", "usb", "static", or an address
string(TOLOWER "${FRC_PREFER_TARGET}" FRC_PREFER_TARGET)
set(target_mdns "roboRIO-${TEAM_NUMBER}-FRC.local")
set(target_usb "172.22.11.2")
set(target_static "10.${TEAM_NUMBER_HIGH}.${TEAM_NUMBER_LOW}.2")
set(_target_preferred "${target_${FRC_PREFER_TARGET}}")
set(TRY_TARGETS)
if(NOT("${_target_preferred}" STREQUAL ""))
    list(APPEND TRY_TARGETS ${_target_preferred})
elseif(NOT("${FRC_PREFER_TARGET}" STREQUAL ""))
    # Use FRC_PREFER_TARGET as an address
    list(APPEND TRY_TARGETS ${FRC_PREFER_TARGET})
endif()
list(APPEND TRY_TARGETS ${target_mdns} ${target_usb} ${target_static})
list(REMOVE_DUPLICATES TRY_TARGETS)

set(USERNAME lvuser)
set(DEPLOY_DIR /home/lvuser)
set(TMP_SSH_HOSTS tmp_ssh_hosts.txt)
set(SSH_FLAGS
    -C
    -o StrictHostKeyChecking=no
    -o UserKnownHostsFile=${TMP_SSH_HOSTS}
    -o LogLevel=error
)
file(WRITE ${TMP_SSH_HOSTS} "")

function(log MESSAGE)
    message("==> ${MESSAGE}")
endfunction()

function(run_ssh MESSAGE EXE)
    log("${MESSAGE}")
    list(REMOVE_AT ARGV 0 1)
    string(TOLOWER ${EXE} EXE)
    if(${EXE} STREQUAL ssh)
        set(EXE ${SSH_EXECUTABLE})
    elseif(${EXE} STREQUAL scp)
        set(EXE ${SCP_EXECUTABLE})
    else()
        message(FATAL_ERROR "invalid executable: ${EXE} (needs ssh/scp)")
    endif()
    execute_process(COMMAND ${EXE} ${SSH_FLAGS} ${ARGV} RESULT_VARIABLE STATUS)
    if (NOT(${STATUS} EQUAL 0))
        message(FATAL_ERROR "SSH failed")
    endif()
endfunction()

set(TARGET)
foreach(CUR_TARGET ${TRY_TARGETS})
    log("Trying ${CUR_TARGET}")
    execute_process(COMMAND ${SSH_EXECUTABLE} ${SSH_FLAGS} ${USERNAME}@${CUR_TARGET} true
        RESULT_VARIABLE SSH_RESULT
        TIMEOUT 5
    )
    if ("${SSH_RESULT}" EQUAL 0)
        set(TARGET ${CUR_TARGET})
        break()
    endif()
endforeach()
if (NOT TARGET)
    message(FATAL_ERROR "Could not find target")
endif()
log("Connected to ${TARGET}")

run_ssh("Deleting robot code"
    ssh
    ${USERNAME}@${TARGET}
    "rm -f ${DEPLOY_DIR}/FRCUserProgram"
)

run_ssh("Copying over robot code (compressed)"
    scp
    ${ROBOT_EXECUTABLE}.gz
    ${USERNAME}@${TARGET}:${DEPLOY_DIR}/FRCUserProgram.gz
)
run_ssh("Decompressing"
    ssh
    ${USERNAME}@${TARGET}
    "gunzip ${DEPLOY_DIR}/FRCUserProgram.gz"
)
run_ssh("Stopping netconsole-host"
    ssh
    ${USERNAME}@${TARGET}
    "killall -q netconsole-host || :"
)
run_ssh("Copying over robot start script"
    scp
    ${WPILIB_HOME}/ant/robotCommand
    ${USERNAME}@${TARGET}:${DEPLOY_DIR}
)
run_ssh("Restarting robot"
    ssh
    ${USERNAME}@${TARGET}
    ". /etc/profile.d/natinst-path.sh\\\\;"
    "chmod a+x ${DEPLOY_DIR}/FRCUserProgram\\\\;"
    "/usr/local/frc/bin/frcKillRobot.sh -t -r\\\\;"
    sync
)
log("Done.")
file(REMOVE ${TMP_SSH_HOSTS})
