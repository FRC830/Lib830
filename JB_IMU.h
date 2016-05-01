/*
 *  SPI Interface for JB Robotics IMU.
 *
 *  The IMU returns roll, pitch, yaw, rolldot, pitchdot, yawdot, accel_x, accel_y, accel_z
 *  as 2 byte integer values in units of 100ths of degrees for roll pitch and yaw, and as
 *  angular velocity in 10ths of deg/sec for rolldot, pitchdot and yawdot.
 */

#pragma once

#include "WPILib.h"

#define BIN_SIZE (19)

#define	IMU_TO_DEG(x)	(((float)x)/100.0)		// IMU angle i in deg*100
#define IMU_TO_VEL(x)	(((float)x)/10.0)		// IMU angular velocity is in deg/s*10

class JB_IMU {
public:
	JB_IMU();
	float GetRoll();
	float GetPitch();
	float GetYaw();
	void Update();
	void Start();
	void Stop();

private:
	void UpdateData();
	void Calibrate();

	static const unsigned char JB_DATA_SIZE = 6; //6 bytes of SPI data to be received (for now)
	//static const unsigned char PARITY_BIT = 1; //parity check on first bit
	//static const unsigned char FIRST_BYTE_DATA = 0x3; //mask to find sensor data bits on first byte: X X X X X X D D
	//static const unsigned char THIRD_BYTE_DATA = 0xFC; //mask to find sensor data bits on third byte: D D D D D D X X

	//static const unsigned char READ_COMMAND = 0x20; //0010 0000 for first byte

	float roll;
	float pitch;
	float yaw;

	unsigned char command[JB_DATA_SIZE];	// SPI data transmitted, in this case always 0
	unsigned char data[JB_DATA_SIZE];		// SPI data received, 3 2 byte values for roll, pitch and yaw in units of 100ths of a degree
	SPI * spi;
};
