/*
 * PwmControl.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#include "PwmControl.h"

PwmControl::PwmControl(GPIO_TypeDef* gpio, uint8_t outputPin) : gpio(gpio), outputPin(outputPin) {

	IoDriver::initPin(gpio, vector<uint8_t>({outputPin}), GpioMode::altPushPullOutput);

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->CCMR1 |= 0x60 | TIM_CCMR1_OC1FE;
	TIM3->CCR2 |= 0x70;
	TIM3->ARR = 256;
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

	while (1) {
		uint16_t desiredSpeedValue = desiredSpeed->get().rpm;
		uint16_t currentSpeedValue = currentSpeed->get().rpm;
		uint16_t diff = desiredSpeedValue - currentSpeedValue;
		if (diff  > maxMotorRpm) {
			diff = 0;
		}
		TIM3->CCR1 = 0xff * diff / desiredSpeedValue;
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
PwmControl::~PwmControl() {
	// TODO Auto-generated destructor stub
}

