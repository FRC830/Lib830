set(LIB830_SRCS
    components/DigitalLED.cpp
    components/EncoderWrapper.cpp

    dashboard/SelectWidget.cpp

    extensions/Gyro.cpp

    input/Gamepad.cpp
    input/GamepadF310.cpp

    logging/Logging.cpp

    util/Algorithms.cpp
    util/ID.cpp
    util/Threading.cpp
    util/Version.cpp
)
add_library(830 STATIC ${LIB830_SRCS})
target_link_libraries(830 wpi)
target_compile_definitions(830 PRIVATE "-DCMAKE_PROJECT_NAME=\"${PROJECT_NAME}\"")
