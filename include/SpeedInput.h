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
#include "RotationState.h"
using namespace::std;

class SpeedInput {
private:
	GPIO_TypeDef* gpio;
	uint8_t inputPin;

	RotationState speedState;
	StateHolder<RotationState>* stateHolder;

	long maxRpm;

public:
	SpeedInput(GPIO_TypeDef* gpio, uint8_t inputPin);

	void setStateHolder(StateHolder<RotationState>* stateHolder);
	void setMaxRpm(uint16_t maxRpm);

	void run();

};


#endif /* SPEEDControl_H_ */
