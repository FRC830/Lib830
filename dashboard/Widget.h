#pragma once

#include <string>

#include "frc/smartdashboard/SmartDashboard.h"

namespace Lib830 {

	class Widget {
	public:
		virtual void sendToDashboard (std::string id) = 0;
	};

}
