/*
 * LedDisplayDriver.cpp
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */
#include "LedDisplayDriver.h"

void LedDisplayDriver::init() {

//	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	IoDriver::initOutput(GPIOB, kI2cPins, GpioMode::altPushPullOutput);

	uint8_t clockFrequency = 8;
	I2C1->CR2 = I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | clockFrequency;
	I2C1->TRISE = clockFrequency + 1;

	I2C1->CR1 |= I2C_CR1_PE;
}

void LedDisplayDriver::start() {

	I2C1->CR1 |= I2C_CR1_START;
}

void LedDisplayDriver::writeByte(uint8_t data) {
	USART1->DR = data;
}

extern "C" void I2C1_EV_IRQHandler(void) {

}

extern "C" void I2C1_ER_IRQHandler(void) {

}
