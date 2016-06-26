#pragma once

#include <atomic>
#include <string>
#include <thread>

class AnalogGyro;
class ADXRS450_Gyro;

namespace Lib830 {

	template <class GyroType>
	class Gyro : public GyroType {
	private:
		std::atomic<bool> calibrating;
	public:
		// pass all constructor arguments to a GyroType constructor
		template<class... Args>
		Gyro(Args&&... args) : GyroType(args...), calibrating(false) {}

		void UpdateTable() override {
			auto table = GyroType::GetTable();
			if (table != nullptr) {
				table->PutNumber("angle", GyroType::GetAngle());
				table->PutNumber("rate", GyroType::GetRate());
				if (table->GetBoolean("reset", false)) {
					GyroType::Reset();
					table->PutBoolean("reset", false);
				}
				if (!calibrating && table->GetBoolean("calibrate", false)) {
					Calibrate();
				}
			}
		}

		void Calibrate() override {
			if (!calibrating) {
				GyroType::GetTable()->PutBoolean("calibrate", true);
				calibrating = true;
				std::thread([&]{
					GyroType::Calibrate();
					calibrating = false;
					GyroType::GetTable()->PutBoolean("calibrate", false);
				}).detach();
			}
		}
	};

	typedef Gyro<AnalogGyro> AnalogGyro;
	typedef Gyro<ADXRS450_Gyro> ADXRS450_Gyro;

}
