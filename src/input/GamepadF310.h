#pragma once

#include "WPILib.h"

namespace Lib830 {
namespace Input {

namespace F310Buttons {
	//the gamepad buttons
	const int A = 1;
	const int B = 2;
	const int X = 3;
	const int Y = 4;
	const int LeftBumper = 5;
	const int RightBumper = 6;
	const int Back = 7;
	const int Start = 8;
	const int LeftStick = 9;
	const int RightStick = 10;
	const int DPadUp = 11;
	const int DPadDown = 12;
	const int DPadLeft = 13;
	const int DPadRight = 14;
	const int Min = A;
	const int Max = DPadRight;
}

class GamepadF310 {
public:
	static constexpr float STICK_THRESHOLD = 0.01;
	GamepadF310(int port);
	virtual ~GamepadF310();
	float LeftX();
	float LeftY();
	float LeftTrigger();
	float RightTrigger();
	float RightX();
	float RightY();
	float DPadX(); //only returns +1, 0, or -1
	float DPadY(); //only returns +1, 0, or -1

	void LeftRumble(float rumbleness); //doesn't actually do anything on our controllers :(
	void RightRumble(float rumbleness); //ditto

	struct ButtonEvent {
		bool pressed;
		int button;
	};
	bool ButtonState(int buttonNum);
	bool GetButtonEvent(ButtonEvent *e);

	// whether either left or right x/y are above STICK_THRESHOLD
	bool AnyStick();

protected:
	std::vector<bool> button_state;
	static const int LEFT_X_AXIS_NUM = 0;
	static const int LEFT_Y_AXIS_NUM = 1;
	static const int LEFT_TRIGGER_NUM = 2;
	static const int RIGHT_TRIGGER_NUM = 3;
	static const int RIGHT_X_AXIS_NUM = 4;
	static const int RIGHT_Y_AXIS_NUM = 5;

	Joystick * joystick;
};

}}
