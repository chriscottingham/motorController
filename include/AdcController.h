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
	uint16_t values[9];
};

class AdcController {
private:
	int nextChannelIndex;
	AdcState adcState;
	StateHolder<AdcState>* stateHolder;

public:
	AdcController();
	virtual ~AdcController(){};

	void setStateHolder(StateHolder<AdcState>* stateHolder);
	void addChannel(GPIO_TypeDef* gpio, int channel);
	void startAdc();
	void run();
};

#endif /* ADCCONTROLLER_H_ */
