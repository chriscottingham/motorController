/*
 * SpeedControl.h
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#ifndef SPEEDControl_H_
#define SPEEDControl_H_

#include "IoDriver.h"
#include "StateHolder.h"

using namespace::std;

struct SpeedInputState {
	SpeedInputState(uint16_t inputSpeed) : inputSpeed(inputSpeed) {}
	uint16_t inputSpeed;
};

class SpeedInput {
private:
	GPIO_TypeDef* gpio;
	uint8_t inputPin;

	SpeedInputState speedState;
	StateHolder<SpeedInputState>* stateHolder;

	uint16_t maxSpeed = 3600;

public:
	SpeedInput(GPIO_TypeDef* gpio, uint8_t inputPin);

	void setStateHolder(StateHolder<SpeedInputState>* stateHolder);
	void setMaxSpeed(uint16_t maxSpeed);

	void run();

};


#endif /* SPEEDControl_H_ */
