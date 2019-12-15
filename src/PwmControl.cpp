/*
 * PwmControl.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#include "PwmControl.h"
#include <cmath>

#define MAX_ARR 0xffff

#define MS_CYCLE_DELAY 5

PwmControl::PwmControl(GPIO_TypeDef* gpio, uint8_t outputPin) : gpio(gpio), outputPin(outputPin) {

	IoDriver::initPin(gpio, vector<uint8_t>({outputPin}), GpioMode::altPushPullOutput);

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->CCMR1 |= 0x60 | TIM_CCMR1_OC1FE;
//	TIM3->CCR2 |= 0x70;
	TIM3->ARR = MAX_ARR;
	TIM3->CCER |= 0x01;

	TIM3->CR1 |= TIM_CR1_CEN;

}

void PwmControl::setCurrentSpeedHolder(StateHolder<RotationState>* currentSpeed) {
	this->currentSpeed = currentSpeed;
}
void PwmControl::setDesiredSpeedHolder(StateHolder<RotationState>* desiredSpeed) {
	this->desiredSpeed = desiredSpeed;
}

void PwmControl::setMaxMotorRpm(long maxRpm) {
	this->maxMotorRpm = maxRpm;
}

void PwmControl::setAdcChannel(int channel) {
	this->adcChannel = channel;
}

void PwmControl::run() {

	static int previousValue = 0;
	while (1) {
		uint16_t desiredSpeedValue = desiredSpeed->get().rpm;
		uint16_t currentSpeedValue = currentSpeed->get().rpm;
		uint16_t diff = desiredSpeedValue - currentSpeedValue;
		if (diff  > maxMotorRpm) {
			diff = 0;
		}
		uint32_t onPercentage = 5 * MAX_ARR * diff / desiredSpeedValue;
		if (onPercentage > MAX_ARR) {
			onPercentage = MAX_ARR;
		}
		float i = 0.02;
		onPercentage = i * onPercentage + (1 - i) * previousValue;

		uint16_t currentAdc = adcState->get().values[adcChannel - 2];
		float currentLimitFactor = 1 - std::pow(3.3f * currentAdc / MAX_ARR, 2);
		if (currentLimitFactor < 0) {
			currentLimitFactor = 1;
		}
		onPercentage *= currentLimitFactor;

		if (currentSpeedValue <= 20) {
			lastMotorStoppedDuration += MS_CYCLE_DELAY;
			if (lastMotorStoppedDuration >= 1000) {
				faultMotorStop = true;
			}
		} else {
			lastMotorStoppedDuration = 0;
		}

		if (faultMotorStop) {
			onPercentage = 0;
		}

		TIM3->CCR1 = onPercentage;
		previousValue = onPercentage;

		vTaskDelay(pdMS_TO_TICKS(MS_CYCLE_DELAY));
	}
}

void PwmControl::setAdcStateHolder(StateHolder<AdcState>* adcState) {
	this->adcState = adcState;
}
PwmControl::~PwmControl() {
	// TODO Auto-generated destructor stub
}

