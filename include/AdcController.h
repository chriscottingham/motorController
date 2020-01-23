/*
 * AdcController.h
 *
 *  Created on: Nov 28, 2019
 *      Author: chris
 */

#ifndef ADCCONTROLLER_H_
#define ADCCONTROLLER_H_

#include "IoDriver.h"

class AdcController {
private:
	int* resultBuffer;
	int* channels;
	int channelCount;

public:
	void init(int* resultBuffer, int* channels, int channelCount);
	void startAdc();

	int getChannelValue(int channel);
};

#endif /* ADCCONTROLLER_H_ */
