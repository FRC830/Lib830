/*
 * JB_IMU.h
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

#ifndef UTIL_JB_IMU_H_
#define UTIL_JB_IMU_H_

#include "WPILib.h"

//long roll, pitch, yaw, rolldot, pitchdot, yawdot;
//int accel_x, accel_y, accel_z;
#define BIN_SIZE (19)

#define	IMU_TO_DEG(x)	(((float)x)/100.0)		// IMU angle i in deg*100
#define IMU_TO_VEL(x)	(((float)x)/10.0)		// IMU angular velocity is in deg/s*10

//int JB_IMU_GyroUpdateFunction(int pointer_val);

class JB_IMU {
public:
	JB_IMU();
	float GetRoll();
	float GetPitch();
	float GetYaw();
	//float GetRate();
	//float GetAngle();
	void Reset();
	void Update();
	//float Offset();
	void Start();
	void Stop();

private:
	void UpdateData();
	void Calibrate();

	//static void check_parity(unsigned char * command); //gyro requires odd parity for command
	//static int bits(unsigned char val); //returns number of on bits in a byte (helper for parity check)

	//static short assemble_sensor_data(unsigned char * data); //takes the sensor data from the data array and puts it into an int

	static const unsigned char JB_DATA_SIZE = 6; //6 bytes of SPI data to be received (for now)
	//static const unsigned char PARITY_BIT = 1; //parity check on first bit
	//static const unsigned char FIRST_BYTE_DATA = 0x3; //mask to find sensor data bits on first byte: X X X X X X D D
	//static const unsigned char THIRD_BYTE_DATA = 0xFC; //mask to find sensor data bits on third byte: D D D D D D X X

	//static const unsigned char READ_COMMAND = 0x20; //0010 0000 for first byte

	float roll;
	float pitch;
	float yaw;

	//float accumulated_angle;
	Timer * update_timer;
	Timer * calibration_timer;
	//float current_rate;
	//float accumulated_offset;
	//float rate_offset;

	unsigned char command[JB_DATA_SIZE];	// SPI data transmitted, in this case always 0
	unsigned char data[JB_DATA_SIZE];		// SPI data received, 3 2 byte values for roll, pitch and yaw in units of 100ths of a degree
	SPI * spi;

	Task * update_task;
	bool task_started;

	//char sensor_output_1[9];
	//char sensor_output_2[9];
	//char sensor_output_3[9];
	//char sensor_output_4[9];
};

#endif /* UTIL_ADXRS450GYRO_H_ */
