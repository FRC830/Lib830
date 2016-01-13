/*
 * Team 830, the RatPack's, utility classes for programming robots in C++.
 * Includes:
 * GamepadF310: provides interface for the F310 gamepad
 * AccelerationController: can be used to control rate at which a variable increases
 *
 */


#ifndef UTIL_830_UTILITIES_H_
#define UTIL_830_UTILITIES_H_

#include "GamepadF310.h"
#include "ADXRS450Gyro.h"
#include "DigitalLED.h"
#include "Math.h"
#include "Misc.h"

void reverse(char s[]); //reverses the order of characters in s
void to_binary_string(unsigned char n, char s[]); //store a string in s corresponding to the binary representation of the value of n
int normalize_angle(float angle); //returns an equivalent angle between 0 and 360

#endif
