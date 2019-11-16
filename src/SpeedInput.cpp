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
//	IoDriver::initPin(GPIOA, vector<uint8_t>{2}, GpioMode::analogInput);
	GPIOA->CRL &= ~(0xf<<8);

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->SMPR2 |= 0x1c;
	ADC1->SQR3 |= inputPin;
	ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON | ADC_CR2_ALIGN;

	ADC1->CR2 |= ADC_CR2_ADON;
}

void SpeedInput::setStateHolder(StateHolder<RotationState>* stateHolder) {
	this->stateHolder = stateHolder;
}

void SpeedInput::setMaxRpm(uint16_t maxRpm) {
	this->maxRpm = maxRpm;
}

void SpeedInput::run() {
	while (1) {
		volatile long masked = (0xff00 & ADC1->DR) >> 8;
		speedState.rpm = masked * maxRpm / 256;
		stateHolder->set(speedState);

		vTaskDelay(pdMS_TO_TICKS(250));

	}
}
