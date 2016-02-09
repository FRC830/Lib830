/*
 * Math.h
 *
 *  Created on: Nov 15, 2015
 *      Author: ratpack
 */

#ifndef UTIL_MATH_H_
#define UTIL_MATH_H_

#include <algorithm>
#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

float accel(float current, float target, uint8_t ticks);

#endif /* UTIL_MATH_H_ */
