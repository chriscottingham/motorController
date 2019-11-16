/*
 * RotaryEncoder.h
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <vector>

#include "IoDriver.h"
#include "StateHolder.h"
#include "RotationState.h"

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
	uint16_t previousEncoderCount = 0;

	StateHolder<RotationState>* encoderStateHolder;

	void updateSpeed();

public:
	RotaryEncoder(GPIO_TypeDef* timerPort, vector<uint8_t>* const pins);

	void setEncoderStateHolder(StateHolder<RotationState>* encoderStateHolder);
	Direction getDirection();
	void run();
};

void RotaryEncoderTask(void* param);

#endif /* ROTARYENCODER_H_ */
