/*
 * AdcController.h
 *
 *  Created on: Nov 28, 2019
 *      Author: chris
 */

#ifndef ADCCONTROLLER_H_
#define ADCCONTROLLER_H_

#include "IoDriver.h"
#include "StateHolder.h"

struct AdcState {
	int values[9];
};

class AdcController {
private:
	int nextChannelIndex = 0;
	AdcState adcState;
	StateHolder<AdcState>* stateHolder;

public:
	virtual ~AdcController(){};

	void setStateHolder(StateHolder<AdcState>* stateHolder);
	void addChannel(GPIO_TypeDef* gpio, int channel);
	void startAdc();
	void runTask();
};

#endif /* ADCCONTROLLER_H_ */
