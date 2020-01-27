/*
 * SpeedControl.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#include <SpeedInput.h>

SpeedInput::SpeedInput(uint8_t adcValueIndex) : adcValueIndex(adcValueIndex){
}

void SpeedInput::setMaxRpm(uint16_t maxRpm) {
	this->maxRpm = maxRpm;
}

void SpeedInput::setAdcController(AdcController* adcController) {
	this->adcController = adcController;
}
int SpeedInput::getInputSpeed() {

	static int previousValue = 0;

	uint32_t adcValue = adcController->getChannelValue(adcValueIndex);
	uint16_t masked = 0xffff & adcValue;
	int newValue = masked * maxRpm / 0xffff;

	float integral = 0.7;
	newValue = integral * newValue + (1-integral) * previousValue;
	previousValue = newValue;

	return newValue;
}
