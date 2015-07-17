/*
 * JB_IMU.cpp
 *
 *  Created on: Mar 29, 2015
 *      Author: ratpack
 *
 *  SPI Interface for JB Robotics IMU.
 *
 *  The IMU returns roll, pitch, yaw, rolldot, pitchdot, yawdot, accel_x, accel_y, accel_z
 *  as 2 byte integer values in units of 100ths of degrees for roll pitch and yaw, and as
 *  angular velocity in 10ths of deg/sec for rolldot, pitchdot and yawdot.
 */

#include "JB_IMU.h"
#include "830utilities.h"
#include <cstdarg>


#define  SENSOR1
#ifdef  SENSOR1
// SENSOR1
long sensorAngleBins[BIN_SIZE] =  {
  -18000, -17500, -15400, -13800, -11300, -8800, -5100, -1500, 0, 2100, 4400, 6800, 8700, 10500, 12000, 13700, 15300, 17100, 18000};
long compassAngleBins[BIN_SIZE] = {
  -16800, -15750, -13500, -11250, -9000, -6750, -4500, -2250, -1200, 0, 2250, 4500, 6700, 9000, 11250, 13500, 15700, 18000, 19200};
#endif
#ifdef SENSOR2
// SENSOR2
long sensorAngleBins[BIN_SIZE] =  {
  -18000, -16733, -14850, -12762, -10788, -8607, -5055, -1989, 0, 15.41, 4896, 7529, 9236, 11093, 12171, 14228, 15755, 17599, 18000};
long compassAngleBins[BIN_SIZE] = {
  -17200, -15750, -13500, -11250, -9000, -6750, -4500, -2250, -800, 0, 2250, 4500, 6750, 9000, 11250, 13500, 15700, 18000, 18800};
#endif



// JB_IMU_UpdateFunction
//
// This function implements a task used to periodically read the sensor.  The task is statically defined
// and is externally Started, Suspended or Resumed.  This task is creating a timer and on each pass through
// its loop is calling the Update() function and then passing the task loop time to the smart dash board.
//
// The timer is not being used to control the loop time.
//
// The way this is being done I don't see what is regulating the time between passes through the while loop.
// Perhaps this thing should suspend at the bottom of the loop and some external timer task should resume it
// as needed... or some other approach.
//
// input
//    pointer_val     int       Pointer to allocated instance of JB_IMU
//
// return
//                              Never returns
//
int JB_IMU_UpdateFunction(int pointer_val) {
	JB_IMU * jb_imu = (JB_IMU *) pointer_val;

	// Creates a timer used to measure the time between gyro updates.
	Timer * timer = new Timer(); //don't update too often
	timer->Start();

	// Iterate Infinitely...
	while (true) {
		// Do a spi transaction to read value from the sensor
		jb_imu->Update();

		// Report measured time to smart dashboard
		SmartDashboard::PutNumber("task loop time", timer->Get());

		// Reset timer for next pass
		timer->Reset();
	}
	return 0;
}


// bound
//
// This function takes the raw sensorAngle from the JB_IMU and limits the range from -180 deg to 180 deg.
//
// input
//    sensorAngle   long     Signed value in units of 100ths of degrees.
//
// return
//                  long	 The sensor angle bound to the range of +- 180 degrees
//
long bound(long sensorAngle){
  if (sensorAngle > 18000) {
    sensorAngle -= 36000;
  }
  else if (sensorAngle < -18000) {
    sensorAngle += 36000;
  }
  return sensorAngle;
}

// convert_sensor_to_compass
//
// input
//    long     yawSensorAngle    Sensor angle in hundredths of degrees
//
//    returns  compassAngle      Compass angle in hundredths of degrees
//
long convert_sensor_to_compass(long yawSensorAngle) {
  yawSensorAngle= bound(yawSensorAngle);

  unsigned int count;
  //int compassAngle;
  long deltaSensorError, deltaCompassBin;
  bool done;

  long result = 0;

  //iterates once for each bin until we find the bin the sensor is in
  for (count = 0; (count < BIN_SIZE) && (done == false); count++) {
    int nextCount = count + 1;
    if(nextCount == BIN_SIZE)
      nextCount = 0;
    //checks if this bin contains this sensor angle
    if (yawSensorAngle >= sensorAngleBins[count] && yawSensorAngle <= sensorAngleBins[nextCount]) {
      //does the interpolation to determine the compass value
      int binCompassDelta = compassAngleBins[nextCount] - compassAngleBins[count];
      //compassAngle = compassAngleBins(count
      int sensorDelta = yawSensorAngle - sensorAngleBins[count];
      float percentage = (float)sensorDelta / (float)(sensorAngleBins[nextCount] - sensorAngleBins[count]);
      result = compassAngleBins[count] + binCompassDelta * percentage;
    }
  }

  return bound(result);
}



// JB_IMU::JB_IMU
//
// Constructor which defines which one of the 4 SPI ports to use with the JB_IMU.  It initializes the
// transmit and receive buffers used with the SPI.  It creates a new task which when Started will periodically
// call the JB_IMU_UpdateFunction().
JB_IMU::JB_IMU() {
	spi = new SPI(SPI::kOnboardCS0);
	spi->SetClockRate(4000000); //4 MHz (rRIO max, gyro can go high)
	spi->SetClockActiveHigh();
	spi->SetChipSelectActiveLow();
	spi->SetMSBFirst();

	// Load command array with zeros.  The sensor is not using what we send so zeros OK.
	command[0] = 0;
	command[1] = 0;
	command[2] = 0;
	command[3] = 0;
	command[4] = 0;
	command[5] = 0;

	// PreLoad the buffer of received data bytes with 0.  This will soon be overwritten with received SPI data.
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;

	// Initialize roll, pitch and yaw values to 0
	roll= pitch= yaw= 0.0;

#ifdef JUNK
	accumulated_angle = 0.0;
	current_rate = 0.0;
	accumulated_offset = 0.0;
	rate_offset = 0.0;
	update_timer = new Timer();
	calibration_timer = new Timer();
#endif

	update_task = new Task("JB_IMU_update", (FUNCPTR) &JB_IMU_UpdateFunction); //TODO: this should give a unique name for each gyro object
	task_started = false;
}


void JB_IMU::Start() {
	if (task_started) {
		update_task->Resume();
	} else {
		update_task->Start((int) this);
		task_started = true;
	}
}

void JB_IMU::Stop() {
	if (task_started) {
		update_task->Suspend();
	}
}

void JB_IMU::Update() {

	//calibration_timer->Start();

	//check_parity(command);
	spi->Transaction(command, data, JB_DATA_SIZE); //perform transaction, get error code

	//if (calibration_timer->Get() < 5.0){
	//	return;
	//} else if (calibration_timer->Get() < 15.0) {
	//	Calibrate();
	//} else {
		UpdateData();
	//}

	//to_binary_string(data[0], sensor_output_1);
	//to_binary_string(data[1], sensor_output_2);
	//to_binary_string(data[2], sensor_output_3);
	//to_binary_string(data[3], sensor_output_4);

	//SmartDashboard::PutString("gyro sensor data 1", sensor_output_1);
	//SmartDashboard::PutString("gyro sensor data 2", sensor_output_2);
	//SmartDashboard::PutString("gyro sensor data 3", sensor_output_3);
	//SmartDashboard::PutString("gyro sensor data 4", sensor_output_4);
}
void JB_IMU::UpdateData() {
	long raw_roll;	// roll in units of 100ths of degrees, from -180 to 180
	long raw_pitch;	// pitch in units of 100ths of degrees, from -180 to 180
	long raw_yaw;	// yaw in units of 100ths of degrees, from -180 to 180
	long adjusted_yaw; // adjusted yaw in units of 100ths of degrees, from -180 to 180

	// Extract the roll pitch yaw angles from the SPI data
	raw_roll=  (data[0]<<8) + data[1];
	raw_pitch= (data[2]<<8) + data[3];
	raw_yaw=   (data[4]<<8) + data[5];

	// Using the calibration table and interpolation convert the read compass angle into an adjusted
	// compass angle
	adjusted_yaw= convert_sensor_to_compass( raw_yaw);

	// Convert the roll pitch yaw data into float values
	roll=  IMU_TO_DEG(raw_roll);
	pitch= IMU_TO_DEG(raw_pitch);
	yaw=   IMU_TO_DEG(adjusted_yaw);

	// Apply the magnetic compensation by interpolation at 18 points on the compass


	//int sensor_data = assemble_sensor_data(data);

	//float rate = ((float) sensor_data) / 80.0;

	//current_rate = rate;
	//current_rate -= rate_offset;
	//update_timer->Start();
	//accumulated_angle += update_timer->Get() * current_rate;
	//update_timer->Reset();
}

void JB_IMU::Calibrate() {
#ifdef JUNK
	int sensor_data = assemble_sensor_data(data);
	float rate = ((float) sensor_data) / 80.0;
	update_timer->Start();
	calibration_timer->Start();
	accumulated_offset += rate * update_timer->Get();
	rate_offset = accumulated_offset / calibration_timer->Get();
	update_timer->Reset();
#endif
}


float JB_IMU::GetRoll(){
	return roll;
}

float JB_IMU::GetPitch(){
	return pitch;
}

float JB_IMU::GetYaw(){
	return yaw;
}

#ifdef	JUNK
float JB_IMU::GetRate() {
	return current_rate;
}

float JB_IMU::GetAngle() {
	return accumulated_angle;
}

float JB_IMU::Offset() {
	return rate_offset;
}
#endif

void JB_IMU::Reset() {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	//current_rate = 0.0;
	//accumulated_angle = 0.0;
	//rate_offset = 0.0;
	//accumulated_offset = 0.0;
	calibration_timer->Stop();
	calibration_timer->Reset();
	update_timer->Stop();
	update_timer->Reset();
}

#ifdef	JUNK
short JB_IMU::assemble_sensor_data(unsigned char * data){
	//cast to short to make space for shifts
	//the 16 bits from the gyro are a 2's complement short
	//so we just cast it too a C++ short
	//the data is split across the output like this (MSB first): (D = data bit, X = not data)
	// X X X X X X D D | D D D D D D D D | D D D D D D X X | X X X X X X X X X
	return ((short) (data[0] & FIRST_BYTE_DATA)) << 14
		| ((short) data[1]) << 6
		| ((short) (data[2] & THIRD_BYTE_DATA)) >> 2;
}

void JB_IMU::check_parity(unsigned char * command) {
	int num_bits = bits(command[0]) + bits(command[1]) + bits(command[2]) + bits(command[3]);
	if (num_bits % 2 == 0) {
		command[3] |= PARITY_BIT;
	}
}

int JB_IMU::bits(unsigned char val) {
	int n = 0;
	while (val) {
		val &= val-1;
		n += 1;
	}
	return n;
}
#endif
