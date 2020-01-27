/*
 * MotorDisplay.cpp
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */
#include <MotorDisplay.h>
#include <string>
#include <stdio.h>
#include <functional>

#include "LiberationMono_32.h"
#include "Timer.hpp"
#include "MemberMethodInvoker.hpp"
#include "System.hpp"

#define FONT_HEIGHT 32

uint8_t MotorDisplay::initializationSequence[] = {
				COMMAND, 0xa8, 0x3f,
				COMMAND, 0xd3, 0,
				COMMAND, 0x40,
				COMMAND, 0xa1,
				COMMAND, 0xc8,
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

MotorDisplay::MotorDisplay() : timer(Timer(100, bind(&MotorDisplay::offerRun, this))){

	IoDriver::initPin(GPIOB, {10,11}, GpioMode::pushPullOutput);
	System::getInstance().registerTickListener(&timer);

//	font32_chars[45] = font32_45;
	font32_chars[48] = font32_48;
	font32_chars[49] = font32_49;
	font32_chars[50] = font32_50;
	font32_chars[51] = font32_51;
	font32_chars[52] = font32_52;
	font32_chars[53] = font32_53;
	font32_chars[54] = font32_54;
	font32_chars[55] = font32_55;
	font32_chars[56] = font32_56;
	font32_chars[57] = font32_57;
	font32_chars[58] = font32_58;
	font32_chars[67] = font32_67;
	font32_chars[84] = font32_84;
}

void MotorDisplay::handleI2cInterrupt() {

	static bool isMaster = false;

	uint16_t statusRegister = I2C1->SR1;

	if (statusRegister & I2C_SR1_SB) {
		I2C1->DR = 0x78;
	} else if (statusRegister & I2C_SR1_ADDR) {
		isMaster = I2C1->SR2 & 1;
		if (isMaster) {
			DMA1_Channel6->CCR |= DMA_CCR1_EN;
//			I2C1->CR2 |= I2C_CR2_DMAEN;
		}
	}
}

void MotorDisplay::handleI2cError() {

	static char buffer[8];

	std::basic_string<char> output("I2C error: SR1:");

	sprintf(buffer, "%04X", I2C1->SR1);
	output.append(buffer);

	output.append(", SR2:");

	sprintf(buffer, "%04X", I2C1->SR2);
	output.append(buffer);

	trace_puts(output.c_str());

	stop();
	NVIC_ClearPendingIRQ(I2C1_ER_IRQn);
}

void MotorDisplay::handleDma() {

	if (DMA1->ISR & DMA_ISR_TEIF6) {
		Logger::write("dma6Handler");
		Logger::writeln(DMA1->ISR);
		stop();
	} else if (DMA1->ISR & DMA_ISR_TCIF6) {
		stop();
	}
	DMA1->IFCR |= DMA_ISR_GIF6;
}


void MotorDisplay::initI2c() {

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	IoDriver::initPin(kI2cGpio, kI2cPins, GpioMode::altOpenDrainOutput);

	for (IRQn_Type interrupt : { I2C1_ER_IRQn, I2C1_EV_IRQn }) {
		NVIC_ClearPendingIRQ(interrupt);
		NVIC_EnableIRQ(interrupt);
	}
}

void MotorDisplay::initDma() {

	RCC->AHBENR |= RCC_AHBPeriph_DMA1;

	for (IRQn_Type interrupt : { DMA1_Channel6_IRQn }) {
		NVIC_ClearPendingIRQ(interrupt);
		NVIC_EnableIRQ(interrupt);
	}

	DMA_Channel_TypeDef* channel = DMA1_Channel6;
	channel->CPAR = I2C1_BASE + 0x10;
	channel->CCR |= 0x3000 | DMA_CCR1_MINC | DMA_CCR1_DIR | DMA_CCR1_TEIE | DMA_CCR1_TCIE;

//	channel->CCR |= DMA_CCR1_EN;
}

void MotorDisplay::startI2c() {

	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	uint8_t clockFrequency = 8;
	I2C1->CR2 = I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | clockFrequency;
	I2C1->TRISE = 1;
//	I2C1->CCR = 40; //SCL master clock
	I2C1->CCR = 10; //SCL master clock

	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 |= I2C_CR2_DMAEN;
	I2C1->CR1 |= I2C_CR1_START;
}

void MotorDisplay::resetBuffer() {

	displayBuffer[0] = DATA;
	memset(&displayBuffer[1], 0, 1024);
}

void copyString(uint8_t* target, uint8_t* source, uint8_t length) {
	for (uint8_t i=0; i<length; i++) {
		target[i] = source[i];
	}
}

void MotorDisplay::drawBuffer() {

	displayBuffer[0] = DATA;

	Point<int> firstLineEndPoint = drawChars("C:", 2, Point<int>(0,0));
//	firstLineEndPoint = drawNumber(encoderStateHolder->get().rpm, 4, Point<int>(firstLineEndPoint.getX(), 0));
	firstLineEndPoint = drawNumber(2143, 4, Point<int>(firstLineEndPoint.getX(), 0));

	Point<int> secondLineEndPoint = drawChars("T:", 2, Point<int>(0, firstLineEndPoint.getY()));
//	drawNumber(speedInputStateHolder->get().rpm, 4, Point<int>(secondLineEndPoint.getX(), firstLineEndPoint.getY()));
	drawNumber(1324, 4, Point<int>(secondLineEndPoint.getX(), firstLineEndPoint.getY()));

	drawPowerBar();
}

void MotorDisplay::drawPowerBar() {

	uint8_t powerByte = TIM3->CCR1;

	int iRow;
	int columnWidth = 20;
	for (iRow=7; iRow>=7 - powerByte / 32; iRow--) {
		for (int iColumn = 0; iColumn < columnWidth; iColumn++) {
			displayBuffer[1 + (iRow * 128) + (128 - columnWidth) + iColumn] = 0xff;
		}
	}

	iRow = 7 - powerByte / 32;
	uint8_t barPortion = powerByte % 8;
	uint16_t barMask = 0Xff << (8 - barPortion);
	for (int iColumn = 0; iColumn < columnWidth; iColumn++) {
		displayBuffer[1 + iRow * 128 + (128 - columnWidth) + iColumn] = barMask;
	}

}

Point<int> MotorDisplay::drawNumber(int value, int charCount, Point<int> topLeft) {

	char charValue[charCount + 1];

	snprintf(charValue, charCount+1, "%04d", value);
	return drawChars(charValue, charCount, topLeft);
}

Point<int> MotorDisplay::drawChars(char* source, uint8_t charCount, Point<int> topLeft) {

	int endX = 0;

	int bufferOffset = 1 + 128 * topLeft.getY() / (FONT_HEIGHT / 4) + topLeft.getX();

	for (uint8_t iChar=0; iChar<charCount; iChar++) {

		volatile uint8_t charIndex=source[iChar];
		int charWidth = font32_width[charIndex - 33];

		for (int iRow=0; iRow<4; iRow++) {

			for (int iCharColumn=0; iCharColumn < charWidth; iCharColumn++) {

				displayBuffer[bufferOffset + iRow*128 + endX + iCharColumn] =
						font32_chars[charIndex][iRow*charWidth + iCharColumn];
			}
		}
		endX += charWidth;
	}

	return Point<int>(topLeft.getX() + endX, topLeft.getY() + FONT_HEIGHT);
}

void MotorDisplay::sendBuffer() {
	DMA1_Channel6->CCR &= ~DMA_CCR1_EN;
	DMA1_Channel6->CMAR = (uint32_t) &displayBuffer;
	DMA1_Channel6->CNDTR = 1025;
//	DMA1_Channel6->CCR |= DMA_CCR1_CIRC;
	DMA1_Channel6->CCR |= DMA_CCR1_EN;

//	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 |= I2C_CR2_DMAEN;
	I2C1->CR1 |= I2C_CR1_START;
}

void MotorDisplay::stop() {
	while (!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1->CR1 |= I2C_CR1_STOP;
	I2C1->CR2 &= ~I2C_CR2_DMAEN;
//	I2C1->CR1 &= ~I2C_CR1_PE;
}

void MotorDisplay::offerRun() {

	if (timer.isReady()) {

		GPIOB->ODR ^= GPIO_ODR_ODR11;

		if (!bitValues.powerOn) {
			IoDriver::initPin(kPowerGpio, kPowerPin, GpioMode::pushPullOutput);
			kI2cGpio->ODR |= GPIO_ODR_ODR9;
			resetBuffer();
			bitValues.powerOn = true;

		} else if (!bitValues.displayInitialized) {
			initDma();
			initI2c();
			DMA1_Channel6->CMAR = (uint32_t) &initializationSequence;
			DMA1_Channel6->CNDTR = sizeof(initializationSequence);
			startI2c();
			bitValues.displayInitialized = true;

		} else if (bitValues.displayDirty) {
			resetBuffer();
			drawBuffer();
			sendBuffer();
		}

		GPIOB->ODR ^= GPIO_ODR_ODR11;

		timer.reset();
	}
}
