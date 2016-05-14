set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/arm-toolchain.cmake)
set(FRC_DEPLOY_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/frc-deploy.cmake)

set(WPILIB_HOME $ENV{HOME}/wpilib/cpp/current)
include_directories(${WPILIB_HOME}/include)
link_directories(${WPILIB_HOME}/lib)

function(add_frc_executable _NAME)
    add_executable(${ARGV})
    target_link_libraries(${_NAME} libwpi.so)
endfunction()

function(add_frc_deploy TARGET_NAME TEAM_NUMBER ROBOT_EXECUTABLE)
    find_program(SSH_EXECUTABLE ssh)
    find_program(SCP_EXECUTABLE scp)

    if(SSH_EXECUTABLE AND SCP_EXECUTABLE)
        add_custom_target(${TARGET_NAME}
            COMMAND ${CMAKE_COMMAND}
                -D TEAM_NUMBER=${TEAM_NUMBER}
                -D ROBOT_EXECUTABLE=${ROBOT_EXECUTABLE}
                -D SSH_EXECUTABLE=${SSH_EXECUTABLE}
                -D SCP_EXECUTABLE=${SCP_EXECUTABLE}
                -D WPILIB_HOME=${WPILIB_HOME}
                -P ${FRC_DEPLOY_SCRIPT}
            DEPENDS ${PROJECT_NAME}
            COMMENT "Deploying"
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
