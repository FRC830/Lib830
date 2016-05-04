#include "Algorithms.h"

namespace Lib830 {
namespace Util {

float accel(float current, float target, uint8_t ticks) {
	if (std::abs(target) <= 0.05) {
		target = 0;
	}
	if (ticks == 0) {
		return target;
	}

	if ((target > 0 && current < 0) || (target < 0 && current > 0)) {
		return 0;
	}

	float max_delta = 1.0 / ticks;
	float delta = target - current;

	if (delta > max_delta) {
		return current + max_delta;
	}
	else if (delta < -max_delta) {
		return current - max_delta;
	}
	else {
		return target;
	}
}

}}
