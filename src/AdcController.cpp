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

AdcController::AdcController() : nextChannelIndex(0) {
}

void AdcController::setStateHolder(StateHolder<AdcState>* stateHolder) {
	this->stateHolder = stateHolder;
}

void AdcController::addChannel(GPIO_TypeDef* gpio, int channel) {

//		IoDriver::initPin(GPIOA, std::vector<uint8_t>{2}, GpioMode::analogInput);
		gpio->CRL &= ~(0xffff<<8); //cheating for channels 2 and 3 (0xf << 4*3 + 0xf << 4*2)

		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
		RCC->AHBENR |= RCC_AHBPeriph_DMA1;

//		ADC1->SMPR2 |= 1 << 20;
		ADC1->CR1 |= ADC_CR1_SCAN;
		ADC1->SQR1 |= 1<<20;
		ADC1->SQR3 |= 2 | 3<<5; //channel numbers

		DMA1_Channel1->CCR |= 1 << 12 | 1 << 10 | 1<<8 | DMA_CCR1_MINC | DMA_CCR1_CIRC;
		DMA1_Channel1->CNDTR = 2;
		DMA1_Channel1->CPAR = 0x4001244c;
		DMA1_Channel1->CMAR = (uint32_t) adcState.values;

//		Logger::write(DMA1_Channel1->CMAR);

		ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON | ADC_CR2_ALIGN | ADC_CR2_DMA;
}

void AdcController::startAdc() {

	DMA1_Channel1->CCR |= DMA_CCR1_EN;
	ADC1->CR2 |= ADC_CR2_ADON;
}

void AdcController::run() {

	for (int i=0; i<9; i++) {
		adcState.values[i] = 0;
	}
	startAdc();
	while (true) {
		stateHolder->set(adcState);
		adcState.values[4] = ADC1->DR;
		vTaskDelay(pdMS_TO_TICKS(5));
	}
}
