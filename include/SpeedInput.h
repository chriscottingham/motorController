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
#include "AdcController.h"

using namespace::std;

class SpeedInput {
private:
	int adcChannel;

	RotationState speedState;
	StateHolder<RotationState>* stateHolder;
	StateHolder<AdcState>* adcStateHolder;

	long maxRpm;

public:
	SpeedInput();

	void setAdcStateHolder(StateHolder<AdcState>* adcStateHolder);
	void setStateHolder(StateHolder<RotationState>* stateHolder);
	void setMaxRpm(uint16_t maxRpm);
	void setAdcChannel(int adcChannel);

	void run();

};


#endif /* SPEEDControl_H_ */
