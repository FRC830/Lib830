#pragma once

#include "WPILib.h"
#include "Gamepad.h"

namespace Lib830 {

	class GamepadF310 : public Gamepad {
	public:
		GamepadF310() = delete;
		GamepadF310(int port);
		virtual ~GamepadF310();

		float LeftX();
		float LeftY();
		float LeftTrigger();
		float RightTrigger();
		float RightX();
		float RightY();

		int DPadX(); // only returns +1, 0, or -1
		int DPadY(); // only returns +1, 0, or -1
		inline bool DPadUp() { return DPadY() == 1 && DPadX() == 0; }
		inline bool DPadDown() { return DPadY() == -1 && DPadX() == 0; }
		inline bool DPadLeft() { return DPadY() == 0 && DPadX() == -1; }
		inline bool DPadRight() { return DPadY() == 0 && DPadX() == 1; }

		bool AnyStickPressed();
		bool AnyTriggerPressed();

		enum Button {
			BUTTON_A = 1,
			BUTTON_B,
			BUTTON_X,
			BUTTON_Y,
			BUTTON_LEFT_BUMPER,
			BUTTON_RIGHT_BUMPER,
			BUTTON_BACK,
			BUTTON_START,
			BUTTON_LEFT_STICK,
			BUTTON_RIGHT_STICK,
		};
		enum Axis {
			AXIS_LEFT_X = 0,
			AXIS_LEFT_Y,
			AXIS_LEFT_TRIGGER,
			AXIS_RIGHT_TRIGGER,
			AXIS_RIGHT_X,
			AXIS_RIGHT_Y,
		};

	};

}
