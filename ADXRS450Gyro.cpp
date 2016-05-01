#include "830utilities.h"
#include "ADXRS450Gyro.h"
#include "Threading.h"
#include <cstdarg>
#include <set>

static LockedItem<std::set<ADXRS450Gyro*> > active_gyros;

int ADXRS450GyroUpdateFunction(int) {
	while (true) {
		LockedItemGuard g(active_gyros.mutex);
		for_each(active_gyros.value, [](ADXRS450Gyro *gyro) {
			gyro->Update();
		});
		std::this_thread::yield();
	}
	return 0;
}

ADXRS450Gyro::ADXRS450Gyro() {
	spi = new SPI(SPI::kOnboardCS0);
	spi->SetClockRate(4000000); //4 MHz (rRIO max, gyro can go high)
	spi->SetClockActiveHigh();
	spi->SetChipSelectActiveLow();
	spi->SetMSBFirst();

	command[0] = READ_COMMAND;
	command[1] = 0;
	command[2] = 0;
	command[3] = 0;
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	accumulated_angle = 0.0;
	current_rate = 0.0;
	accumulated_offset = 0.0;
	rate_offset = 0.0;
	update_timer = new Timer();
	calibration_timer = new Timer();

	static bool started = false;
	if (!started) {
		new Task("adxrs450update", (FUNCPTR)&ADXRS450GyroUpdateFunction);
		started = true;
	}
}


void ADXRS450Gyro::Start() {
	LockedItemGuard g(active_gyros.mutex);
	active_gyros.value.insert(this);
}

void ADXRS450Gyro::Stop() {
	LockedItemGuard g(active_gyros.mutex);
	active_gyros.value.erase(this);
}

void ADXRS450Gyro::Update() {

	calibration_timer->Start();

	check_parity(command);
	spi->Transaction(command, data, DATA_SIZE); //perform transaction, get error code

	if (calibration_timer->Get() < 5.0){
		return;
	} else if (calibration_timer->Get() < 15.0) {
		Calibrate();
	} else {
		UpdateData();
	}
}

void ADXRS450Gyro::UpdateData() {
	int sensor_data = assemble_sensor_data(data);

	float rate = ((float) sensor_data) / 80.0;

	current_rate = rate;
	current_rate -= rate_offset;
	update_timer->Start();
	accumulated_angle += update_timer->Get() * current_rate;
	update_timer->Reset();
}

void ADXRS450Gyro::Calibrate() {
	int sensor_data = assemble_sensor_data(data);
	float rate = ((float) sensor_data) / 80.0;
	update_timer->Start();
	calibration_timer->Start();
	accumulated_offset += rate * update_timer->Get();
	rate_offset = accumulated_offset / calibration_timer->Get();
	update_timer->Reset();
}



float ADXRS450Gyro::GetRate() {
	return current_rate;
}

float ADXRS450Gyro::GetAngle() {
	return accumulated_angle;
}

float ADXRS450Gyro::Offset() {
	return rate_offset;
}

void ADXRS450Gyro::Reset() {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	current_rate = 0.0;
	accumulated_angle = 0.0;
	rate_offset = 0.0;
	accumulated_offset = 0.0;
	calibration_timer->Stop();
	calibration_timer->Reset();
	update_timer->Stop();
	update_timer->Reset();
}

short ADXRS450Gyro::assemble_sensor_data(unsigned char * data){
	//cast to short to make space for shifts
	//the 16 bits from the gyro are a 2's complement short
	//so we just cast it too a C++ short
	//the data is split across the output like this (MSB first): (D = data bit, X = not data)
	// X X X X X X D D | D D D D D D D D | D D D D D D X X | X X X X X X X X X
	return ((short) (data[0] & FIRST_BYTE_DATA)) << 14
		| ((short) data[1]) << 6
		| ((short) (data[2] & THIRD_BYTE_DATA)) >> 2;
}

void ADXRS450Gyro::check_parity(unsigned char * command) {
	int num_bits = bits(command[0]) + bits(command[1]) + bits(command[2]) + bits(command[3]);
	if (num_bits % 2 == 0) {
		command[3] |= PARITY_BIT;
	}
}

int ADXRS450Gyro::bits(unsigned char val) {
	int n = 0;
	while (val) {
		val &= val-1;
		n += 1;
	}
	return n;
}
