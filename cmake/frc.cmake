set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/arm-toolchain.cmake)

set(WPILIB_HOME $ENV{HOME}/wpilib/cpp/current)
include_directories(${WPILIB_HOME}/include)
link_directories(${WPILIB_HOME}/lib)

function(add_frc_executable _NAME)
    add_executable(${ARGV})
    target_link_libraries(${_NAME} libwpi.so)
endfunction()

function(add_frc_deploy TARGET_NAME TEAM_NUMBER ROBOT_EXECUTABLE)
    set(TARGET roboRIO-${TEAM_NUMBER}-FRC.local)
    set(USERNAME lvuser)
    set(DEPLOY_DIR /home/lvuser)

    find_program(SSH_EXECUTABLE ssh)
    find_program(SCP_EXECUTABLE scp)

    if(SSH_EXECUTABLE AND SCP_EXECUTABLE)
        add_custom_target(${TARGET_NAME}
            COMMAND ssh
            ${USERNAME}@${TARGET}
            rm -f ${DEPLOY_DIR}/FRCUserProgram

            COMMAND scp
            ${ROBOT_EXECUTABLE}
            ${USERNAME}@${TARGET}:${DEPLOY_DIR}/FRCUserProgram

            COMMAND ssh
            ${USERNAME}@${TARGET}
            killall -q netconsole-host || :

            COMMAND scp
            ${WPILIB_HOME}/ant/robotCommand
            ${USERNAME}@${TARGET}:${DEPLOY_DIR}

            COMMAND ssh
            ${USERNAME}@${TARGET}
            . /etc/profile.d/natinst-path.sh;
            chmod a+x ${DEPLOY_DIR}/FRCUserProgram;
            /usr/local/frc/bin/frcKillRobot.sh -t -r;
            sync

            DEPENDS ${PROJECT_NAME}
            COMMENT "Deploying to ${TARGET}"
        )
        set_target_properties(${TARGET_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    else()
        message(FATAL_ERROR "Could not deploy! ssh/scp executables not found!")
    endif()
endfunction()

function(add_frc_robot NAME TEAM_NUMBER)
    list(REMOVE_AT ARGV 0 1)
    add_frc_executable(${NAME} ${ARGV})
    add_frc_deploy(deploy ${TEAM_NUMBER} $<TARGET_FILE:${NAME}>)
endfunction()
