/*
 * AdcController.cpp
 *
 *  Created on: Nov 28, 2019
 *      Author: chris
 */

#include "Logger.h"

#include "AdcController.h"
#include "IoDriver.h"

AdcController::AdcController(GPIO_TypeDef* gpio, const vector<uint8_t>& channels) {

	this->resultBuffer = new uint16_t[channels.size()];
	this->channels = channels;

		gpio->CRL &= ~(0xff<<8); //cheating for channels 2 and 3 (0xf << 4*3 + 0xf << 4*2);

	RCC->AHBENR |= RCC_AHBPeriph_DMA1;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	DMA1_Channel1->CNDTR = channels.size();

	ADC1->SQR1 &= ~(0xf << 20);
	ADC1->SQR1 |= channels.size() << 20;

	for (uint8_t i=0; i<channels.size(); i++) {
//		IoDriver::initPin(gpio, std::vector<uint8_t>{channels[i]}, GpioMode::analogInput);
		ADC1->SQR3 |= channels[i] << (5 * i); //channel numbers
	}

	DMA1_Channel1->CCR |= 1 << 12 | 1 << 10 | 2 << 8 | DMA_CCR1_MINC | DMA_CCR1_CIRC;
	DMA1_Channel1->CPAR = 0x4001244c;
	DMA1_Channel1->CMAR = (uint32_t) resultBuffer;

//	ADC1->CR1 |= ADC_CR1_SCAN;
}

int AdcController::getChannelValue(int channel) {
	return resultBuffer[channel];
}

void AdcController::startAdc() {

	DMA1_Channel1->CCR |= DMA_CCR1_EN;

	ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_ADON | ADC_CR2_ALIGN | ADC_CR2_DMA;
	ADC1->CR2 |= ADC_CR2_ADON;
}
