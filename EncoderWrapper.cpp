/*
 * EncoderWrapper.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: ratpack
 */

#include "EncoderWrapper.h"

EncoderWrapper::EncoderWrapper(Encoder * enc, double upMult, double downMult) {
	encoder = enc;
	previousEncoderValue = enc->Get();
	upMultiplier = upMult;
	downMultiplier = downMult;
	adjustedValue = previousEncoderValue;
}

void EncoderWrapper::update(){
	int encValue = encoder->Get();
	int diff = encValue - previousEncoderValue;
	if(diff > 0){
		adjustedValue += diff * upMultiplier;
	}
	else if(diff < 0){
		adjustedValue -= diff * downMultiplier;
	}

	previousEncoderValue = encValue;
}

void EncoderWrapper::reset(){
	adjustedValue = 0;
	encoder->Reset();
}

double EncoderWrapper::value(){
	return adjustedValue;
}

int EncoderWrapper::rawValue(){
	return encoder->Get();
}
