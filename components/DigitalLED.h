#pragma once

#include "WPILib.h"

namespace Lib830 {

	class DigitalLED {
	private:
		DigitalOutput * red_channel;
		DigitalOutput * green_channel;
		DigitalOutput * blue_channel;
		Timer timer;
		void Enable();

	public:
		struct Color { //RGB
			double red;
			double green;
			double blue;
		};


		DigitalLED(DigitalOutput * red, DigitalOutput * green, DigitalOutput * blue);
		DigitalLED(int red, int green, int blue);

		void Set(double red, double green, double blue);
		void Set(Color color);
		void Disable();
		void Alternate(Color color_1, Color color_2);
		void SetAllianceColor();
	};

}
