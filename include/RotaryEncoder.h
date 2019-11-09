/*
 * RotaryEncoder.h
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include "micro.h"

struct DirectionEnum {
	enum type {
		FORWARD, REVERSE
	};
};
typedef TypedEnum<DirectionEnum> Direction;

class RotaryEncoder {
private:
	GPIO_TypeDef* timerPort;
	uint8_t encoderPins[2];

public:
	RotaryEncoder(GPIO_TypeDef* timerPort, uint8_t encoderPins[2]);

	uint16_t getRpm();
	Direction getDirection();
	void tick();
};

#endif /* ROTARYENCODER_H_ */
