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

#include "diag/Trace.h"

class LedDisplayDriver {

public:
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

public:
	static uint8_t initializationSequence[];

	const std::vector<uint8_t> kI2cPins = { 6, 7 };

	char displayBuffer[128];

	void initI2c();
	void initDma();

	void handleInterrupt();

	void startI2c();
	void stop();

	void buttonPressed();

	void writeByte(uint8_t value);

	void i2cDmaError();


};



#endif /* LEDDISPLAYDRIVER_H_ */
