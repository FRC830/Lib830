#include "DigitalLED.h"
#include "DigitalOutput.h"
#include "Timer.h"
#include <cmath>

namespace Lib830 {

	void DigitalLED::Enable() {
		red_channel->EnablePWM(0.0);
		green_channel->EnablePWM(0.0);
		blue_channel->EnablePWM(0.0);
	}

	DigitalLED::DigitalLED(DigitalOutput * red, DigitalOutput * green, DigitalOutput * blue) {
		red_channel = red;
		green_channel = green;
		blue_channel = blue;
		Enable();
	}

	DigitalLED::DigitalLED(int red, int green, int blue) {
		red_channel = new DigitalOutput(red);
		green_channel = new DigitalOutput(green);
		blue_channel = new DigitalOutput(blue);
		Enable();
	}


	void DigitalLED::Set( double red, double green, double blue){
		red_channel->UpdateDutyCycle(red);
		green_channel->UpdateDutyCycle(green);
		blue_channel->UpdateDutyCycle(blue);
		SmartDashboard::PutNumber("red", red_channel->Get());
		SmartDashboard::PutNumber("green", green_channel->Get());
		SmartDashboard::PutNumber("blue", blue_channel->Get());

	}

	void DigitalLED::Set(Color color) {
		Set(color.red, color.green, color.blue);
	}

	void DigitalLED::Disable() {
		Set(0,0,0);
	}

	void DigitalLED::Alternate(Color color_1, Color color_2) {
		timer.Start();
		int time = timer.Get();
		if (time%2 == 0) {
			Set(color_1);
		}
		else {
			Set(color_2);
		}
	}

	void DigitalLED::SetAllianceColor() {
		auto color = DriverStation::GetInstance().GetAlliance();
		if (color == DriverStation::kBlue) {
			Set(0,0,1);
		}
		else if (color == DriverStation::kRed) {
			Set(1,0,0);
		}
		else {
			Set(0,0,0);
		}
	}

	void DigitalLED::RainbowFade(float fade_duration) {
		timer.Start();
		double x = timer.Get() *(pi/(fade_duration/2));
		double r = 0.5*sin(x) + 0.5;
		double g = (0.5*sin(x - ((2*pi)/3))) + 0.5;
		double b =  (0.5*sin(x - ((4*pi)/3))) + 0.5;
		Set(r,g,b);
	}

	DigitalLED::Color DigitalLED::hex2color(int hex) {
		Color color;
		color.red = ((hex &0xff0000) >> 16 )/255.0;
		color.green = ((hex &0x00ff00) >> 8)/255.0;
		color.blue = ((hex &0x0000ff))/255.0;
		return color;
	}
	void DigitalLED::Set(int hex) {
		Color color = hex2color(hex);
		Set(color);
	}
}
