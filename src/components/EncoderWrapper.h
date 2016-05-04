#pragma once

#include "WPILib.h"

namespace Lib830 {
namespace Components {

class EncoderWrapper {
public:
	EncoderWrapper(Encoder * encoder, double upMult, double downMult);
	void reset();
	void update();
	double value();
	int rawValue();
private:
	double upMultiplier;
	double downMultiplier;
	Encoder * encoder;
	int previousEncoderValue;
	double adjustedValue;
};

}}
