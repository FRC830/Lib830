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

set(USERNAME lvuser)
set(DEPLOY_DIR /home/lvuser)

set(TARGET)
foreach(CUR_TARGET "roboRIO-${TEAM_NUMBER}-FRC.local" "172.22.11.2" "10.${TEAM_NUMBER_HIGH}.${TEAM_NUMBER_LOW}.2")
    message("Trying ${CUR_TARGET}")
    execute_process(COMMAND ${SSH_EXECUTABLE} ${USERNAME}@${CUR_TARGET} rm -f ${DEPLOY_DIR}/FRCUserProgram
        RESULT_VARIABLE SSH_RESULT
        TIMEOUT 5
    )
    if (${SSH_RESULT} EQUAL 0)
        set(TARGET ${CUR_TARGET})
        break()
    endif()
endforeach()
if (NOT TARGET)
    message(FATAL_ERROR "Could not find target")
endif()
message("Connected to ${TARGET}")

function(run_ssh_safe MESSAGE)
    message("${MESSAGE}")
    list(REMOVE_AT ARGV 0)
    execute_process(COMMAND ${ARGV} RESULT_VARIABLE STATUS)
    if (NOT(${STATUS} EQUAL 0))
        message(FATAL_ERROR "SSH failed")
    endif()
endfunction()

run_ssh_safe("Copying over robot code"
    ${SCP_EXECUTABLE}
        ${ROBOT_EXECUTABLE}
        ${USERNAME}@${TARGET}:${DEPLOY_DIR}/FRCUserProgram
)
run_ssh_safe("Stopping netconsole-host"
    ${SSH_EXECUTABLE}
        ${USERNAME}@${TARGET}
        killall -q netconsole-host || :
)
run_ssh_safe("Copying over robot start script"
    ${SCP_EXECUTABLE}
        ${WPILIB_HOME}/ant/robotCommand
        ${USERNAME}@${TARGET}:${DEPLOY_DIR}
)
run_ssh_safe("Restarting robot"
    ${SSH_EXECUTABLE}
        ${USERNAME}@${TARGET}
        . /etc/profile.d/natinst-path.sh;
        chmod a+x ${DEPLOY_DIR}/FRCUserProgram;
        /usr/local/frc/bin/frcKillRobot.sh -t -r;
        sync
)
message("Done.")
