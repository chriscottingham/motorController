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

class LedDisplayDriver {

public:
	const std::vector<uint8_t> kI2cPins = { 6, 7 };

	void init();
	void start();

	void writeByte(uint8_t value);
};



#endif /* LEDDISPLAYDRIVER_H_ */
