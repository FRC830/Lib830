/*
 * Team 830, the RatPack's, utility classes for programming robots in C++.
 * Includes:
 * GamepadF310: provides interface for the F310 gamepad
 * AccelerationController: can be used to control rate at which a variable increases
 *
 */

#pragma once

#ifdef __GNUC__
    #if (__GNUC__ == 4 && __GNUC_MINOR__ < 6) || (__GNUC__ < 4)
        #error "Lib830 requires GCC 4.6 or newer"
    #endif
#else
    #warning "Lib830 has not been tested with non-GCC compilers"
#endif

#include <cstdint>

#include "components/DigitalLED.h"

#include "dashboard/Select.h"

#include "input/GamepadF310.h"

#include "logging/Logging.h"

#include "utils/Algorithms.h"
#include "utils/Threading.h"
