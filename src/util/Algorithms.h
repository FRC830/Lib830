#pragma once

#include <algorithm>
#include <cstdint>

namespace Lib830 {

	typedef int8_t int8;
	typedef int16_t int16;
	typedef int32_t int32;
	typedef int64_t int64;
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

	float accel(float current, float target, uint8_t ticks);

	// less-redundant std::for_each
	template <class Container, class Function>
	Function for_each(Container &c, Function f) {
		return std::for_each(c.begin(), c.end(), f);
	}

}
