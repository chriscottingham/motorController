/*
 * SpeedControl.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#include <SpeedControl.h>

SpeedControl::SpeedControl(GPIO_TypeDef* gpio, uint8_t inputPin) : gpio(gpio) {
	this->inputPin = inputPin;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->SMPR2 |= 0x1c;
	ADC1->SQR3 |= inputPin;
	ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON;

	ADC1->CR2 |= ADC_CR2_ADON;
}
