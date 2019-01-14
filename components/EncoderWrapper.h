#pragma once

#include "frc/Encoder.h"

namespace Lib830 {

	class EncoderWrapper {
	public:
		EncoderWrapper(frc::Encoder * encoder, double upMult, double downMult);
		void reset();
		void update();
		double value();
		int rawValue();
	private:
		double upMultiplier;
		double downMultiplier;
		frc::Encoder * encoder;
		int previousEncoderValue;
		double adjustedValue;
	};

}
