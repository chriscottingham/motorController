/*
 * LedDisplayDriver.cpp
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */
#include "LedDisplayDriver.h"

void LedDisplayDriver::init() {

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	IoDriver::initPin(GPIOB, kI2cPins, GpioMode::altOpenDrainOutput);

	uint8_t clockFrequency = 8;
	I2C1->CR2 = I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | clockFrequency;
	I2C1->TRISE = clockFrequency + 1;
//	I2C1->CCR = 32;

	I2C1->CR1 |= I2C_CR1_PE;

	for (IRQn_Type interrupt : { I2C1_ER_IRQn, I2C1_EV_IRQn }) {
		NVIC_ClearPendingIRQ(interrupt);
		NVIC_EnableIRQ(interrupt);
	}
}

void LedDisplayDriver::start() {

	I2C1->CR1 |= I2C_CR1_START;
}

void LedDisplayDriver::writeByte(uint8_t data) {
	USART1->DR = data;
}

bool sendData() {

	bool dataSent = true;

	static std::vector<uint8_t> data = { 0x40, 0xc0, 0xa5, 0x5a, 0xa5, 0x5a, 0xa5, 0x5a, 0x8f };
	static uint8_t dataPosition = 0;

	I2C1->DR = data[dataPosition++];
	if (dataPosition >= data.size()) {
		dataPosition = 0;
		dataSent = false;
	}
	I2C1->CR1 |= I2C_CR1_ACK;

	return dataSent;
}

extern "C" void I2C1_EV_IRQHandler(void) {

	static bool isMaster = false;

	uint16_t statusRegister = I2C1->SR1;

	if (statusRegister & I2C_SR1_SB) {
		sendData();
	} else if (statusRegister & I2C_SR1_ADDR) {
		isMaster = I2C1->SR2 & 1;
		if (isMaster) {
			if (!sendData()) {
				I2C1->CR1 |= I2C_CR1_STOP;
			}
		}
	}
}

extern "C" void I2C1_ER_IRQHandler(void) {

}
