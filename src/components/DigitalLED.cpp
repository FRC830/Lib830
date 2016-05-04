#include "DigitalLED.h"

namespace Lib830 {
namespace Components {

DigitalLED::DigitalLED(DigitalOutput * red, DigitalOutput * green, DigitalOutput * blue) {
	red_channel = red;
	green_channel = green;
	blue_channel = blue;
}

DigitalLED::DigitalLED(int red, int green, int blue) {
	red_channel = new DigitalOutput(red);
	green_channel = new DigitalOutput(green);
	blue_channel = new DigitalOutput(blue);
}

void DigitalLED::Set(enum rgb_color color) {
	Set((bool) (color & kRed), (bool) (color & kGreen), (bool) (color & kBlue)); //mask to find red, green, and blue values
}

void DigitalLED::Set(bool red, bool green, bool blue){
	red_channel->Set(red);
	green_channel->Set(green);
	blue_channel->Set(blue);
}

}}
