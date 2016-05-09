#pragma once

#include <vector>
#include "WPILib.h"

namespace Lib830 {

	class Gamepad {
	public:
		static constexpr float AXIS_THRESHOLD = 0.001;

		Gamepad() = delete;
		Gamepad(int port);
		virtual ~Gamepad();

		bool ButtonState(int button);

		struct Event {
			enum EventType {
				BUTTON_EVENT,
				AXIS_EVENT,
				POV_EVENT,
			};
			EventType type;
			int id;
			bool state; // for button, axis, and POV events
			float value; // for axis events
			int angle; // for POV events
		};
		bool GetEvent(Event &event);
	protected:
		Joystick joystick;
		std::vector<bool> button_state;
		std::vector<float> axis_state;
		std::vector<int> pov_state;
	};

}
