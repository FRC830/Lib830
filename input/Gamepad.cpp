#include "Gamepad.h"

namespace Lib830 {

	Gamepad::Gamepad(int port):
		joystick(port)
	{
		button_state.resize(joystick.GetButtonCount());
		axis_state.resize(joystick.GetAxisCount());
		pov_state.resize(joystick.GetPOVCount());
	}

	Gamepad::~Gamepad() {

	}

	bool Gamepad::ButtonState(int button) {
		return joystick.GetRawButton(button);
	}

	bool Gamepad::GetEvent(Event &event) {
		// button IDs are 1-indexed
		for (size_t button = 1; button <= button_state.size(); ++button) {
			bool state = ButtonState(button);
			if (button_state[button - 1] != state) {
				button_state[button - 1] = state;
				event.type = Event::BUTTON_EVENT;
				event.id = button;
				event.state = state;
				event.value = 0;
				event.angle = 0;
				return true;
			}
		}
		for (size_t axis = 0; axis < axis_state.size(); ++axis) {
			float value = joystick.GetRawAxis(axis);
			if (std::fabs(value - axis_state[axis]) > AXIS_THRESHOLD) {
				axis_state[axis] = value;
				event.type = Event::AXIS_EVENT;
				event.id = axis;
				event.state = (value > AXIS_THRESHOLD);
				event.value = value;
				event.angle = 0;
				return true;
			}
		}
		for (size_t pov = 0; pov < pov_state.size(); ++pov) {
			int angle = joystick.GetPOV(pov);
			if (angle != pov_state[pov]) {
				pov_state[pov] = angle;
				event.type = Event::POV_EVENT;
				event.id = pov;
				event.state = (angle >= 0);  // angle is -1 if not pressed
				event.value = 0;
				event.angle = angle;
				return true;
			}
		}
		return false;
	}

}
