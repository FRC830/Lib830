/*
 * GamepadF310.h
 *
 *  Created on: Jan 5, 2015
 *      Author: ratpack
 */

#include "WPILib.h"

#ifndef UTIL_GAMEPADF310_H_
#define UTIL_GAMEPADF310_H_

class GamepadF310 {
public:
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

	//the gamepad buttons
	const static int buttonA = 1;
	const static int buttonB = 2;
	const static int buttonX = 3;
	const static int buttonY = 4;
	const static int buttonLeftBumper = 5;
	const static int buttonRightBumper = 6;
	const static int buttonBack = 7;
	const static int buttonStart = 8;
	const static int buttonLeftStick = 9;
	const static int buttonRightStick = 10;
	const static int buttonMin = buttonA;
	const static int buttonMax = buttonRightStick;

	struct ButtonEvent {
		bool pressed;
		int button;
	};
	bool ButtonState(int buttonNum);
	bool GetButtonEvent(ButtonEvent *e);

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

#endif /* SRC_GAMEPADF310_H_ */
