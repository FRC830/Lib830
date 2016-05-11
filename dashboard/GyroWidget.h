#pragma once

#include <string>

#include "SmartDashboard/NamedSendable.h"
#include "Widget.h"

template <class GyroType>
class GyroWidget : public GyroType {
public:
	// pass all constructor arguments to a GyroType constructor
	template<class... Args>
	GyroWidget(Args&&... args) : GyroType(args...) {}

	void UpdateTable() {
		auto table = GyroType::GetTable();
		if (table != nullptr) {
			table->PutNumber("angle", GyroType::GetAngle());
			table->PutNumber("rate", GyroType::GetRate());
			if (table->GetBoolean("reset", false)) {
				GyroType::Reset();
				table->PutBoolean("reset", false);
			}
		}
	}
};
