/*
 * LedDisplayDriver.h
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */

#ifndef LEDDISPLAYDRIVER_H_
#define LEDDISPLAYDRIVER_H_

#include "stm32f10x.h"
#include "IoDriver.h"
#include "Logger.h"

//#include "Timer.h"
//#include "task.h"

#include "diag/Trace.h"

#define kI2cGpio GPIOB
#define kPowerGpio GPIOB

class LedDisplayDriver {

public:
	static uint8_t font5x7[];

	enum INSTRUCTION_TYPE {
		COMMAND = 0x00,
		DATA = 0x40
	};

	enum DEVICE_STATE {
		DEVICE_STATE_RESET,
		DEVICE_STATE_INITIALIZED
	};

private:
	DEVICE_STATE runState = DEVICE_STATE_RESET;

	uint8_t lineBuffer[130];
	uint8_t bufferIndex = 0;

public:
	const std::vector<uint8_t> kI2cPins = { 6, 7 };
	const std::vector<uint8_t> kPowerPin = { 9 };

	static uint8_t initializationSequence[];

	char displayBuffer[129];

	void init();
	void initI2c();
	void initDma();

	void startI2c();
	void stop();

	void printHello();

	void dma6Handler();
	void handleI2cInterrupt();

	void resetBuffer();
	void writeChar(uint8_t character);
	void writeToBuffer(char* characters, uint8_t length);
	void writeToBuffer(uint32_t hex32);
	void drawLine();

	void runTask();
};

extern "C" void LedDisplayDriverTask(void*);

#endif /* LEDDISPLAYDRIVER_H_ */
