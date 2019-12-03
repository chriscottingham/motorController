/*
 * PwmControl.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#include "PwmControl.h"

#define MAX_ARR 0xffff

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

void PwmControl::run() {

	static int previousValue = 0;
	while (1) {
		uint16_t desiredSpeedValue = desiredSpeed->get().rpm;
		uint16_t currentSpeedValue = currentSpeed->get().rpm;
		uint16_t diff = desiredSpeedValue - currentSpeedValue;
		if (diff  > maxMotorRpm) {
			diff = 0;
		}
		uint32_t onPercentage = 10 * MAX_ARR * diff / desiredSpeedValue;
		if (onPercentage > MAX_ARR) {
			onPercentage = MAX_ARR;
		}
		float i = 0.02;
		onPercentage = i * onPercentage + (1 - i) * previousValue;

		TIM3->CCR1 = onPercentage;
		previousValue = onPercentage;

		vTaskDelay(pdMS_TO_TICKS(5));
	}
}
PwmControl::~PwmControl() {
	// TODO Auto-generated destructor stub
}

