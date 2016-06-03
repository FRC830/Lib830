#pragma once

#include <string>

class AnalogGyro;
class ADXRS450_Gyro;

namespace Lib830 {

	template <class GyroType>
	class Gyro : public GyroType {
	public:
		// pass all constructor arguments to a GyroType constructor
		template<class... Args>
		Gyro(Args&&... args) : GyroType(args...) {}

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

	typedef Gyro<AnalogGyro> AnalogGyro;
	typedef Gyro<ADXRS450_Gyro> ADXRS450_Gyro;

}
