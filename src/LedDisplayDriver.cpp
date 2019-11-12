/*
 * LedDisplayDriver.cpp
 *
 * font from https://github.com/lexus2k/ssd1306/blob/master/src/ssd1306_fonts.c
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */
#include <string>

#include "LedDisplayDriver.h"

#include "task.h"

uint8_t LedDisplayDriver::font5x7[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, // char ' ' (0x20/32)
    0x00, 0x00, 0x2E, 0x00, 0x00, // char '!' (0x21/33)
    0x06, 0x00, 0x06, 0x00, 0x00, // char '"' (0x22/34)
    0x14, 0x7F, 0x14, 0x7F, 0x14, // char '#' (0x23/35)
    0x06, 0x49, 0x7F, 0x49, 0x30, // char '$' (0x24/36)
    0x24, 0x10, 0x08, 0x24, 0x00, // char '%' (0x25/37)
    0x36, 0x49, 0x36, 0x50, 0x00, // char '&' (0x26/38)
    0x00, 0x00, 0x00, 0x06, 0x00, // char ''' (0x27/39)
    0x00, 0x41, 0x36, 0x00, 0x00, // char '(' (0x28/40)
    0x00, 0x36, 0x41, 0x00, 0x00, // char ')' (0x29/41)
    0x00, 0x08, 0x00, 0x00, 0x00, // char '*' (0x2A/42)
    0x00, 0x08, 0x1C, 0x08, 0x00, // char '+' (0x2B/43)
    0x40, 0x20, 0x00, 0x00, 0x00, // char ',' (0x2C/44)
    0x00, 0x08, 0x08, 0x00, 0x00, // char '-' (0x2D/45)
    0x20, 0x00, 0x00, 0x00, 0x00, // char '.' (0x2E/46)
    0x00, 0x30, 0x06, 0x00, 0x00, // char '/' (0x2F/47)
    0x36, 0x41, 0x41, 0x36, 0x00, // char '0' (0x30/48)
    0x00, 0x00, 0x00, 0x36, 0x00, // char '1' (0x31/49)
    0x30, 0x49, 0x49, 0x06, 0x00, // char '2' (0x32/50)
    0x00, 0x49, 0x49, 0x36, 0x00, // char '3' (0x33/51)
    0x06, 0x08, 0x08, 0x36, 0x00, // char '4' (0x34/52)
    0x06, 0x49, 0x49, 0x30, 0x00, // char '5' (0x35/53)
    0x36, 0x49, 0x49, 0x30, 0x00, // char '6' (0x36/54)
    0x00, 0x01, 0x01, 0x36, 0x00, // char '7' (0x37/55)
    0x36, 0x49, 0x49, 0x36, 0x00, // char '8' (0x38/56)
    0x06, 0x49, 0x49, 0x36, 0x00, // char '9' (0x39/57)
    0x00, 0x14, 0x00, 0x00, 0x00, // char ':' (0x3A/58)
    0x20, 0x14, 0x00, 0x00, 0x00, // char ';' (0x3B/59)
    0x00, 0x08, 0x14, 0x22, 0x00, // char '<' (0x3C/60)
    0x00, 0x14, 0x14, 0x14, 0x00, // char '=' (0x3D/61)
    0x00, 0x22, 0x14, 0x08, 0x00, // char '>' (0x3E/62)
    0x00, 0x01, 0x31, 0x06, 0x00, // char '?' (0x3F/63)
    0x36, 0x49, 0x55, 0x59, 0x2E, // char '@' (0x40/64)
    0x36, 0x09, 0x09, 0x36, 0x00, // char 'A' (0x41/65)
    0x77, 0x49, 0x49, 0x36, 0x00, // char 'B' (0x42/66)
    0x36, 0x41, 0x41, 0x00, 0x00, // char 'C' (0x43/67)
    0x77, 0x41, 0x41, 0x36, 0x00, // char 'D' (0x44/68)
    0x36, 0x49, 0x49, 0x00, 0x00, // char 'E' (0x45/69)
    0x36, 0x09, 0x09, 0x00, 0x00, // char 'F' (0x46/70)
    0x36, 0x41, 0x51, 0x30, 0x00, // char 'G' (0x47/71)
    0x36, 0x08, 0x08, 0x36, 0x00, // char 'H' (0x48/72)
    0x00, 0x00, 0x36, 0x00, 0x00, // char 'I' (0x49/73)
    0x00, 0x40, 0x40, 0x36, 0x00, // char 'J' (0x4A/74)
    0x36, 0x08, 0x14, 0x22, 0x00, // char 'K' (0x4B/75)
    0x36, 0x40, 0x40, 0x00, 0x00, // char 'L' (0x4C/76)
    0x36, 0x01, 0x06, 0x01, 0x36, // char 'M' (0x4D/77)
    0x36, 0x04, 0x10, 0x36, 0x00, // char 'N' (0x4E/78)
    0x36, 0x41, 0x41, 0x36, 0x00, // char 'O' (0x4F/79)
    0x36, 0x09, 0x09, 0x06, 0x00, // char 'P' (0x50/80)
    0x36, 0x41, 0x21, 0x56, 0x00, // char 'Q' (0x51/81)
    0x36, 0x09, 0x19, 0x26, 0x00, // char 'R' (0x52/82)
    0x06, 0x49, 0x49, 0x30, 0x00, // char 'S' (0x53/83)
    0x00, 0x01, 0x37, 0x01, 0x00, // char 'T' (0x54/84)
    0x36, 0x40, 0x40, 0x36, 0x00, // char 'U' (0x55/85)
    0x36, 0x40, 0x36, 0x00, 0x00, // char 'V' (0x56/86)
    0x36, 0x40, 0x30, 0x40, 0x36, // char 'W' (0x57/87)
    0x36, 0x08, 0x08, 0x36, 0x00, // char 'X' (0x58/88)
    0x06, 0x48, 0x48, 0x36, 0x00, // char 'Y' (0x59/89)
    0x20, 0x51, 0x49, 0x45, 0x02, // char 'Z' (0x5A/90)
    0x77, 0x41, 0x41, 0x00, 0x00, // char '[' (0x5B/91)
    0x00, 0x06, 0x30, 0x00, 0x00, // char '\' (0x5C/92)
    0x00, 0x41, 0x41, 0x77, 0x00, // char ']' (0x5D/93)
    0x00, 0x02, 0x01, 0x02, 0x00, // char '^' (0x5E/94)
    0x00, 0x40, 0x40, 0x00, 0x00, // char '_' (0x5F/95)
    0x00, 0x01, 0x02, 0x00, 0x00, // char '`' (0x60/96)
    0x20, 0x54, 0x54, 0x38, 0x40, // char 'a' (0x61/97)
    0x00, 0x36, 0x48, 0x30, 0x00, // char 'b' (0x62/98)
    0x30, 0x48, 0x48, 0x00, 0x00, // char 'c' (0x63/99)
    0x30, 0x48, 0x48, 0x76, 0x00, // char 'd' (0x64/100)
    0x38, 0x54, 0x54, 0x08, 0x00, // char 'e' (0x65/101)
    0x08, 0x6C, 0x0A, 0x00, 0x00, // char 'f' (0x66/102)
    0x08, 0x54, 0x54, 0x38, 0x00, // char 'g' (0x67/103)
    0x36, 0x08, 0x08, 0x30, 0x00, // char 'h' (0x68/104)
    0x00, 0x34, 0x00, 0x00, 0x00, // char 'i' (0x69/105)
    0x00, 0x40, 0x34, 0x00, 0x00, // char 'j' (0x6A/106)
    0x36, 0x10, 0x28, 0x00, 0x00, // char 'k' (0x6B/107)
    0x36, 0x00, 0x00, 0x00, 0x00, // char 'l' (0x6C/108)
    0x30, 0x08, 0x10, 0x08, 0x30, // char 'm' (0x6D/109)
    0x30, 0x08, 0x08, 0x30, 0x00, // char 'n' (0x6E/110)
    0x30, 0x48, 0x48, 0x30, 0x00, // char 'o' (0x6F/111)
    0x78, 0x14, 0x14, 0x08, 0x00, // char 'p' (0x70/112)
    0x08, 0x14, 0x14, 0x68, 0x00, // char 'q' (0x71/113)
    0x30, 0x08, 0x08, 0x00, 0x00, // char 'r' (0x72/114)
    0x08, 0x54, 0x54, 0x20, 0x00, // char 's' (0x73/115)
    0x08, 0x2C, 0x48, 0x00, 0x00, // char 't' (0x74/116)
    0x30, 0x40, 0x40, 0x30, 0x00, // char 'u' (0x75/117)
    0x30, 0x40, 0x30, 0x00, 0x00, // char 'v' (0x76/118)
    0x30, 0x40, 0x20, 0x40, 0x30, // char 'w' (0x77/119)
    0x28, 0x10, 0x10, 0x28, 0x00, // char 'x' (0x78/120)
    0x08, 0x50, 0x50, 0x38, 0x00, // char 'y' (0x79/121)
    0x24, 0x34, 0x2C, 0x24, 0x00, // char 'z' (0x7A/122)
    0x08, 0x36, 0x41, 0x00, 0x00, // char '{' (0x7B/123)
    0x00, 0x36, 0x00, 0x00, 0x00, // char '|' (0x7C/124)
    0x00, 0x41, 0x36, 0x08, 0x00, // char '}' (0x7D/125)
    0x08, 0x08, 0x10, 0x10, 0x00, // char '~' (0x7E/126)
    0x36, 0x41, 0x36, 0x00, 0x00, // char '' (0x7F/127)
};

uint8_t LedDisplayDriver::initializationSequence[] = {
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

void LedDisplayDriver::initI2c() {

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	IoDriver::initPin(kI2cGpio, kI2cPins, GpioMode::altOpenDrainOutput);

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
	channel->CCR |= 0x3000 | DMA_CCR1_MINC | DMA_CCR1_DIR | DMA_CCR1_TEIE | DMA_CCR1_TCIE;

//	channel->CCR |= DMA_CCR1_EN;
}

void LedDisplayDriver::init() {

	IoDriver::initPin(kPowerGpio, kPowerPin, GpioMode::pushPullOutput);
	kI2cGpio->ODR |= GPIO_ODR_ODR9;

	for (uint32_t i=0; i<1000000; i++);

	initDma();
	initI2c();
	DMA1_Channel6->CMAR = (uint32_t) &initializationSequence;
	DMA1_Channel6->CNDTR = sizeof(initializationSequence);
	runState = DEVICE_STATE_INITIALIZED;
}

void LedDisplayDriver::printEncoder() {

  	if (runState == DEVICE_STATE_RESET) {
  		init();
	} else {
		resetBuffer();
		writeToBuffer("hello world", 11);
		writeToBuffer(TIM2->CNT);
		drawLine();
	}
	startI2c();
}

void LedDisplayDriver::dma6Handler() {

	if (DMA1->ISR & DMA_ISR_TEIF6) {
		Logger::write("dma6Handler");
		Logger::writeln(DMA1->ISR);
		stop();
	} else if (DMA1->ISR & DMA_ISR_TCIF6) {
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
//	I2C1->CCR = 40; //SCL master clock
	I2C1->CCR = 10; //SCL master clock

	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 |= I2C_CR2_DMAEN;
	I2C1->CR1 |= I2C_CR1_START;
}

void LedDisplayDriver::resetBuffer() {

	lineBuffer[0] = DATA;
	for (uint8_t i=1; i<130; i++) {
		lineBuffer[i] = 0;
	}
	bufferIndex = 1;
}

void copyString(uint8_t* target, uint8_t* source, uint8_t length) {
	for (uint8_t i=0; i<length; i++) {
		target[i] = source[i];
	}
}

void LedDisplayDriver::writeChar(uint8_t character) {

	copyString(&lineBuffer[bufferIndex], &font5x7[(character - 0x20) * 5], 5);
	bufferIndex += 5;
	lineBuffer[bufferIndex++] = 0;
}

void LedDisplayDriver::writeToBuffer(char* characters, uint8_t length) {

	for (uint8_t i=0; i<length; i++) {
		writeChar(characters[i]);
	}
}

void LedDisplayDriver::writeToBuffer(uint32_t hex32) {

	char buffer[10];
	sprintf(buffer, "0x%08X", hex32);
	for (int i=0; i<10; i++) {
		writeChar(buffer[i]);
	}
}

void LedDisplayDriver::drawLine() {
	DMA1_Channel6->CCR &= ~DMA_CCR1_EN;
	DMA1_Channel6->CMAR = (uint32_t) &lineBuffer;
	DMA1_Channel6->CNDTR = 129;
//	DMA1_Channel6->CCR |= DMA_CCR1_CIRC;
	DMA1_Channel6->CCR |= DMA_CCR1_EN;
}

void LedDisplayDriver::handleI2cInterrupt() {

	static bool isMaster = false;

	uint16_t statusRegister = I2C1->SR1;

	if (statusRegister & I2C_SR1_SB) {
//		trace_puts("I2C started");
		I2C1->DR = 0x78;
	} else if (statusRegister & I2C_SR1_ADDR) {
		isMaster = I2C1->SR2 & 1;
		if (isMaster) {
			DMA1_Channel6->CCR |= DMA_CCR1_EN;
//			I2C1->CR2 |= I2C_CR2_DMAEN;
		}
	}
}

void LedDisplayDriver::handleI2cError() {
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

void LedDisplayDriver::stop() {
	I2C1->CR2 &= ~I2C_CR2_DMAEN;
	while (!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1->CR1 |= I2C_CR1_STOP;
	I2C1->CR1 &= ~I2C_CR1_PE;

}

void LedDisplayDriver::runTask() {

	while (1) {
 		vTaskDelay(pdMS_TO_TICKS(200));
		GPIOB->ODR ^= GPIO_ODR_ODR11;
		GPIOB->ODR ^= GPIO_ODR_ODR10;

		printEncoder();
	}

}

void LedDisplayDriverTask(void* driver) {
	((LedDisplayDriver*) driver)->runTask();
}
