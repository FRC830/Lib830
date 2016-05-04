#pragma once

#include <string>

#include "SmartDashboard/SmartDashboard.h"

namespace Lib830 {

	class Widget {
	public:
		virtual void sendToDashboard (std::string id) = 0;
	};

}
