/*
 * RotaryEncoder.h
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <array>

#include "IoDriver.h"

using namespace std;

struct DirectionEnum {
	enum type {
		FORWARD, REVERSE
	};
};
typedef TypedEnum<DirectionEnum> Direction;

class RotaryEncoder {
private:
	GPIO_TypeDef* timerPort;
	vector<uint8_t> encoderPins;

	uint32_t previousSysTick = 0;
	int previousEncoderCount = 0;


public:
	RotaryEncoder(GPIO_TypeDef* timerPort, const vector<uint8_t>& encoderPins);

	Direction getDirection();
	int getSpeed();
};

void RotaryEncoderTask(void* param);

#endif /* ROTARYENCODER_H_ */
