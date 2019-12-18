/*
 * AdcController.cpp
 *
 *  Created on: Nov 28, 2019
 *      Author: chris
 */

#include "Logger.h"

#include "AdcController.h"
#include <task.h>
#include "IoDriver.h"

AdcController::AdcController() {
		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
		RCC->AHBENR |= RCC_AHBPeriph_DMA1;
}

void AdcController::setStateHolder(StateHolder<AdcState>* stateHolder) {
	this->stateHolder = stateHolder;
}

void AdcController::addChannel(GPIO_TypeDef* gpio, int channel) {

//		IoDriver::initPin(gpio, std::vector<uint8_t>{channel}, GpioMode::analogInput);
//		gpio->CRL &= ~(0xff<<8); //cheating for channels 2 and 3 (0xf << 4*3 + 0xf << 4*2)


	ADC1->SQR1 |= (nextChannelIndex) << 20;
	ADC1->SQR3 |= channel << (5 * nextChannelIndex); //channel numbers
	DMA1_Channel1->CNDTR += 1;
	nextChannelIndex++;
}

void AdcController::startAdc() {

		ADC1->CR1 |= ADC_CR1_SCAN;

		DMA1_Channel1->CCR |= 1 << 12 | 1 << 10 | 1<<8 | DMA_CCR1_MINC | DMA_CCR1_CIRC;
		DMA1_Channel1->CPAR = 0x4001244c;
		DMA1_Channel1->CMAR = (uint32_t) adcState.values;

		ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON | ADC_CR2_ALIGN | ADC_CR2_DMA;

	DMA1_Channel1->CCR |= DMA_CCR1_EN;
	ADC1->CR2 |= ADC_CR2_ADON;
}

void AdcController::run() {

	for (int i=0; i<9; i++) {
		adcState.values[i] = 0;
	}
	startAdc();
	while (true) {
		stateHolder->set(&adcState);
		vTaskDelay(pdMS_TO_TICKS(5));
	}
}
