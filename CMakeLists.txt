set(LIB830_SRCS
    components/DigitalLED.cpp
    components/EncoderWrapper.cpp

    dashboard/SelectWidget.cpp

    extensions/Gyro.cpp

    input/Gamepad.cpp
    input/GamepadF310.cpp

    logging/Logging.cpp

    util/Algorithms.cpp
    util/Version.cpp
)
add_library(830 STATIC ${LIB830_SRCS})
target_link_libraries(830 wpi)
