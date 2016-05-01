#pragma once

#include "WPILib.h"

class DigitalLED {
private:
	DigitalOutput * red_channel;
	DigitalOutput * green_channel;
	DigitalOutput * blue_channel;
public:

	enum rgb_color {
		kRed = 1,
		kGreen = 1 << 1,
		kBlue = 1 << 2,
		kYellow = kRed | kGreen,
		kMagenta = kRed | kBlue,
		kCyan = kGreen | kBlue,
		kWhite = kRed | kBlue | kGreen,
		kOff = 0
	};

	DigitalLED(DigitalOutput * red, DigitalOutput * green, DigitalOutput * blue);
	DigitalLED(int red, int green, int blue);
	void Set(enum rgb_color color);
	void Set(bool red, bool green, bool blue);
};
