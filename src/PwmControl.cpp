/*
 * PwmControl.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#include "PwmControl.h"

PwmControl::PwmControl(GPIO_TypeDef* gpio, uint8_t outputPin) : gpio(gpio), outputPin(outputPin) {

	IoDriver::initPin(gpio, vector<uint8_t>({outputPin}), GpioMode::altPushPullOutput);

	TIM3->CCMR1 |= TIM_CCMR1_OC1M | TIM_CCMR1_OC1FE;

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

	while (1) {
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
PwmControl::~PwmControl() {
	// TODO Auto-generated destructor stub
}

