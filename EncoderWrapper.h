/*
 * EncoderWrapper.h
 *
 *  Created on: Feb 23, 2016
 *      Author: ratpack
 */

#ifndef WPIUTILS_ENCODERWRAPPER_H_
#define WPIUTILS_ENCODERWRAPPER_H_

#include "WPILib.h"
class EncoderWrapper {
public:
	EncoderWrapper(Encoder * encoder, double upMult, double downMult);
	void reset();
	void update();
	double value();
	int rawValue();
private:
	double upMultiplier;
	double downMultiplier;
	Encoder * encoder;
	int previousEncoderValue;
	double adjustedValue;
};

#endif /* WPIUTILS_ENCODERWRAPPER_H_ */
