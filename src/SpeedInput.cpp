/*
 * SpeedControl.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#include <SpeedInput.h>

#include "task.h"

SpeedInput::SpeedInput() : speedState(RotationState(2341)){
}

void SpeedInput::setStateHolder(StateHolder<RotationState>* stateHolder) {
	this->stateHolder = stateHolder;
}

void SpeedInput::setAdcChannel(int adcChannel) {
	this->adcChannel = adcChannel;
}

void SpeedInput::setAdcStateHolder(StateHolder<AdcState>* adcStateHolder) {
	this->adcStateHolder = adcStateHolder;
}

void SpeedInput::setMaxRpm(uint16_t maxRpm) {
	this->maxRpm = maxRpm;
}

void SpeedInput::run() {

	static int previousValue = 0;
	while (1) {
		uint32_t adcValue = adcStateHolder->get().values[adcChannel];
		uint16_t masked = 0xffff & adcValue;
		int newValue = masked * maxRpm / 0xffff;

		float integral = 0.3;
		newValue = integral * newValue + (1-integral) * previousValue;
		previousValue = newValue;

		speedState.rpm = newValue;
		stateHolder->set(&speedState);

		vTaskDelay(pdMS_TO_TICKS(100));

	}
}
