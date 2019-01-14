#include "GamepadF310.h"

using frc::Joystick;

namespace Lib830 {

	GamepadF310::GamepadF310(int port):
		Gamepad(port)
	{

	}

	GamepadF310::~GamepadF310() {

	}


	float GamepadF310::LeftX() {
		return joystick.GetRawAxis(AXIS_LEFT_X);
	}
	float GamepadF310::LeftY() {
		//this one is backwards
		return -joystick.GetRawAxis(AXIS_LEFT_Y);
	}
	float GamepadF310::LeftTrigger() {
		return joystick.GetRawAxis(AXIS_LEFT_TRIGGER);
	}
	float GamepadF310::RightTrigger() {
		return joystick.GetRawAxis(AXIS_RIGHT_TRIGGER);
	}

	float GamepadF310::RightX() {
		return joystick.GetRawAxis(AXIS_RIGHT_X);
	}
	float GamepadF310::RightY() {
		//this one is backwards
		return -joystick.GetRawAxis(AXIS_RIGHT_Y);
	}

	int GamepadF310::DPadX() {
		//the joystick class treats the DPad like a POV switch, i.e. a little control stick
		//it returns an angle giving its position clockwise from straight up
		//on the DPad on the angles will be multiples of 45 degrees
		//this class is written so that the DPad has the same interface as the sticks
		//which is good because the mode button switches the DPad and left stick
		//(which is also why we use <=, even though the physical DPad won't give in-between values)
		int pov = joystick.GetPOV();
		if (45 <= pov && pov <= 135){
			return 1;
		} else if (225 <= pov && pov <= 315) {
			return -1;
		} else {
			return 0;
		}
	}

	int GamepadF310::DPadY() {
		//(see comments on DPadX)

		int pov = joystick.GetPOV();
		if (315 <= pov ||(0 <= pov && pov <= 45) ){
			return 1;
		} else if (135 <= pov && pov <= 225) {
			return -1;
		} else {
			return 0;
		}
	}

	bool GamepadF310::AnyStickPressed() {
		return fabs(LeftX()) > AXIS_THRESHOLD ||
				fabs(LeftY()) > AXIS_THRESHOLD ||
				fabs(RightX()) > AXIS_THRESHOLD ||
				fabs(RightY()) > AXIS_THRESHOLD;
	}

	bool GamepadF310::AnyTriggerPressed() {
		return fabs(LeftTrigger()) > AXIS_THRESHOLD ||
				fabs(RightTrigger()) > AXIS_THRESHOLD;
	}
	void GamepadF310::RumbleLeft(float rumble) {
		joystick.SetRumble(Joystick::kLeftRumble,rumble);
	}

	void GamepadF310::RumbleRight(float rumble) {
		joystick.SetRumble(Joystick::kRightRumble,rumble);
	}
}
