/*
 * SpeedControl.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#include <SpeedInput.h>

#include "task.h"

SpeedInput::SpeedInput(GPIO_TypeDef* gpio, uint8_t inputPin) :
		gpio(gpio), speedState(1234) {
	this->inputPin = inputPin;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->SMPR2 |= 0x1c;
	ADC1->SQR3 |= inputPin;
	ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON;

	ADC1->CR2 |= ADC_CR2_ADON;
}

void SpeedInput::setStateHolder(StateHolder<SpeedInputState>* stateHolder) {
	this->stateHolder = stateHolder;
}

void SpeedInput::setMaxSpeed(uint16_t maxSpeed) {
	this->maxSpeed = maxSpeed;
}

void SpeedInput::run() {
	while (1) {
		speedState.inputSpeed = ADC1->DR * maxSpeed / 256;
		stateHolder->set(speedState);

		vTaskDelay(pdMS_TO_TICKS(50));

	}
}
