/*
 * LedDisplayDriver.cpp
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */
#include "LedDisplayDriver.h"

uint8_t LedDisplayDriver::initializationSequence[] = {
				COMMAND, 0xa8, 0x3f,
				COMMAND, 0xd3, 0,
				COMMAND, 0x40,
				COMMAND, 0xa0,
				COMMAND, 0xc0,
				COMMAND, 0xda, 0x02 | 0x10,
				COMMAND, 0x81, 0x40,
				COMMAND, 0xa4,
				COMMAND, 0xa6,
				COMMAND, 0xd5, 0x80,
				COMMAND, 0x8d, 0x14,
				COMMAND, 0xaf,
				COMMAND, 0x20, //horizontal addressing mode
				COMMAND, 0x21, 0, 127, //column begin, end
				COMMAND, 0x22, 0, 7, //page begin, end
};

uint8_t LedDisplayDriver::sampleData[] = {
		DATA, 0xa5
};
void LedDisplayDriver::initI2c() {

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	IoDriver::initPin(GPIOB, kI2cPins, GpioMode::altOpenDrainOutput);

	for (IRQn_Type interrupt : { I2C1_ER_IRQn, I2C1_EV_IRQn }) {
		NVIC_ClearPendingIRQ(interrupt);
		NVIC_EnableIRQ(interrupt);
	}
}

void LedDisplayDriver::initDma() {

	RCC->AHBENR |= RCC_AHBPeriph_DMA1;

	for (IRQn_Type interrupt : { DMA1_Channel6_IRQn }) {
		NVIC_ClearPendingIRQ(interrupt);
		NVIC_EnableIRQ(interrupt);
	}

	DMA_Channel_TypeDef* channel = DMA1_Channel6;
	channel->CPAR = I2C1_BASE + 0x10;
//	channel->CMAR = (uint32_t) &initializationSequence;
	channel->CCR |= 0x3000 | DMA_CCR1_MINC | DMA_CCR1_DIR | DMA_CCR1_TEIE | DMA_CCR1_TCIE;

//	channel->CCR |= DMA_CCR1_EN;
}

void LedDisplayDriver::buttonPressed() {

  	if (runState == DEVICE_STATE_RESET) {
		initDma();
		initI2c();
		DMA1_Channel6->CMAR = (uint32_t) &initializationSequence;
		DMA1_Channel6->CNDTR = sizeof(initializationSequence);
		runState = DEVICE_STATE_INITIALIZED;
	} else {

		DMA1_Channel6->CCR &= ~DMA_CCR1_EN;
		DMA1_Channel6->CMAR = (uint32_t) &sampleData;
		DMA1_Channel6->CNDTR = 2;
		DMA1_Channel6->CCR |= DMA_CCR1_CIRC;
	}
	startI2c();
}

void LedDisplayDriver::dma6Handler() {

	if (DMA1->IFCR & DMA_ISR_TEIF6) {
		Logger::write("dma6Handler");
		Logger::writeln(DMA1->ISR);
		stop();
	}
	DMA1->IFCR |= DMA_ISR_GIF6;
}

void LedDisplayDriver::startI2c() {

	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	uint8_t clockFrequency = 8;
	I2C1->CR2 = I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | clockFrequency;
	I2C1->TRISE = 1;
	I2C1->CCR = 40; //SCL master clock

	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 |= I2C_CR2_DMAEN;
	I2C1->CR1 |= I2C_CR1_START;
}

void LedDisplayDriver::writeByte(uint8_t data) {
	USART1->DR = data;
}

void LedDisplayDriver::handleI2cInterrupt() {

	static bool isMaster = false;

	uint16_t statusRegister = I2C1->SR1;

	if (statusRegister & I2C_SR1_SB) {
		trace_puts("I2C started");
		I2C1->DR = 0x78;
	} else if (statusRegister & I2C_SR1_ADDR) {
		isMaster = I2C1->SR2 & 1;
		if (isMaster) {
			DMA1_Channel6->CCR |= DMA_CCR1_EN;
//			I2C1->CR2 |= I2C_CR2_DMAEN;
		}
	}
}

void LedDisplayDriver::stop() {
	I2C1->CR2 &= ~I2C_CR2_DMAEN;
	I2C1->CR1 |= I2C_CR1_STOP;
	I2C1->CR1 &= ~I2C_CR1_PE;
}


