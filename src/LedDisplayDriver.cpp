/*
 * LedDisplayDriver.cpp
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */
#include "LedDisplayDriver.h"

#include <string>

uint8_t initCommands[] = {

};

enum DATA_COMMAND_BIT {
	COMMAND,
	DATA
};

void LedDisplayDriver::init() {

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	IoDriver::initPin(GPIOB, kI2cPins, GpioMode::altOpenDrainOutput);
//	GPIOB->CRL &= ~0x11000000;

	for (IRQn_Type interrupt : { I2C1_ER_IRQn, I2C1_EV_IRQn }) {
		NVIC_ClearPendingIRQ(interrupt);
		NVIC_EnableIRQ(interrupt);
	}
}

void LedDisplayDriver::start() {

	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	uint8_t clockFrequency = 8;
	I2C1->CR2 = I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | clockFrequency;
	I2C1->TRISE = 1;
	I2C1->CCR = 40; //SCL master clock

	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR1 |= I2C_CR1_START;
}

void LedDisplayDriver::writeByte(uint8_t data) {
	USART1->DR = data;
}

	static uint8_t dataPosition = 0;

bool sendData() {

	bool dataSent = true;

	uint8_t COMMAND = 0x80;
	uint8_t DATA = 0x40;
	static std::vector<uint8_t> data = {
			0x78,
			COMMAND, 0xa8, 0x3f,
			COMMAND, 0xd3, 0,
			COMMAND, 0x40,
			COMMAND, 0xa0,
			COMMAND, 0xc0,
			COMMAND, 0xda, 0x02 | 0x10,
			COMMAND, 0x81, 0x7f,
			COMMAND, 0xa4,
			COMMAND, 0xa6,
			COMMAND, 0xd5, 0x80,
			COMMAND, 0x8d, 0x14,
			COMMAND, 0xaf,
//			DATA, 0xa5, 0xa5
	};

	I2C1->DR = data[dataPosition++];
	if (dataPosition >= data.size()) {
		dataPosition = 0;
		dataSent = false;
	}
//	I2C1->CR1 |= I2C_CR1_ACK;

	return dataSent;
}

extern "C" void I2C1_EV_IRQHandler(void) {

	static bool isMaster = false;

	uint16_t statusRegister = I2C1->SR1;

	if (statusRegister & I2C_SR1_SB) {
		trace_puts("I2C started");
		sendData();
	} else if (statusRegister & I2C_SR1_ADDR) {
		isMaster = I2C1->SR2 & 1;
		if (isMaster) {
			if (!sendData()) {
				LedDisplayDriver::stop();
			}
		}
	} else {
		if (!sendData()) {
			LedDisplayDriver::stop();
		}
	}
}

void LedDisplayDriver::stop() {
	I2C1->CR1 |= I2C_CR1_STOP;
	I2C1->CR1 &= ~1;
	dataPosition=0;
}

extern "C" void I2C1_ER_IRQHandler(void) {

	static char buffer[8];

	std::basic_string<char> output("I2C error: SR1:");

	sprintf(buffer, "%04X", I2C1->SR1);
	output.append(buffer);

	output.append(", SR2:");

	sprintf(buffer, "%04X", I2C1->SR2);
	output.append(buffer);

	trace_puts(output.c_str());

	LedDisplayDriver::stop();
	NVIC_ClearPendingIRQ(I2C1_ER_IRQn);
}
