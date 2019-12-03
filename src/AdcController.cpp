/*
 * AdcController.cpp
 *
 *  Created on: Nov 28, 2019
 *      Author: chris
 */

#include "AdcController.h"
#include <task.h>

void AdcController::setStateHolder(StateHolder<AdcState>* stateHolder) {
	this->stateHolder = stateHolder;
}

void AdcController::addChannel(GPIO_TypeDef* gpio, int channel) {

	//	IoDriver::initPin(GPIOA, vector<uint8_t>{2}, GpioMode::analogInput);
		gpio->CRL &= ~(0xf<<8);

		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
		ADC1->SMPR2 |= 0x1c;
		ADC1->SQR1 |= 1;
		ADC1->SQR3 |= channel << 4*nextChannelIndex++; //channel numbers
		ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON | ADC_CR2_ALIGN | ADC_CR2_DMA;

		DMA1_Channel1->CCR |= 0x3000 | 0x0800 | DMA_CCR1_MINC | DMA_CCR1_CIRC;
		DMA1_Channel1->CNDTR = 2;
		DMA1_Channel1->CPAR = 0x4001244c;
		DMA1_Channel1->CMAR = adcState.values;
}

void AdcController::startAdc() {

	DMA1_Channel1->CCR |= DMA_CCR1_EN;
	ADC1->CR2 |= ADC_CR2_ADON;
}

void AdcController::runTask() {

	while (true) {
		vTaskDelay(pdMS_TO_TICKS(5));
	}
}
