/*
 * SpeedControl.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#include <SpeedInput.h>

void SpeedInput::setAdcValuePointer(int* adcValuePointer) {
	this->adcValuePointer = adcValuePointer;
}

void SpeedInput::setMaxRpm(uint16_t maxRpm) {
	this->maxRpm = maxRpm;
}

int SpeedInput::getInputSpeed() {

	static int previousValue = 0;

	uint32_t adcValue = *adcValuePointer;
	uint16_t masked = 0xffff & adcValue;
	int newValue = masked * maxRpm / 0xffff;

	float integral = 0.3;
	newValue = integral * newValue + (1-integral) * previousValue;
	previousValue = newValue;

	return newValue;
}
