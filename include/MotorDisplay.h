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

class MotorDisplay {

public:
	enum INSTRUCTION_TYPE {
		COMMAND = 0x00,
		DATA = 0x40
	};

	struct BitValues {
		BitValues() : displayDirty(true) {};
		bool displayDirty:1;
		bool powerOn:1;
		bool displayInitialized:1;
	};

private:
	uint8_t displayBuffer[1025];
	uint16_t rpm = 0;
	BitValues bitValues;

	void initI2c();
	void initDma();

	void resetBuffer();
	void drawBuffer();
	void sendBuffer();

public:
	const std::vector<uint8_t> kI2cPins = { 6, 7 };
	const std::vector<uint8_t> kPowerPin = { 9 };

	static uint8_t initializationSequence[];

	void init();

	void startI2c();
	void stop();

	void handleDma();
	void handleI2cInterrupt();
	void handleI2cError();

	void setSpeed(uint16_t rpm);

	void runTask();
};

extern "C" void MotorDisplayTask(void*);

#endif /* LEDDISPLAYDRIVER_H_ */
