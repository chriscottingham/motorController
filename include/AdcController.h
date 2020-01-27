/*
 * AdcController.h
 *
 *  Created on: Nov 28, 2019
 *      Author: chris
 */

#ifndef ADCCONTROLLER_H_
#define ADCCONTROLLER_H_

#include <vector>
#include "IoDriver.h"

class AdcController {
private:
	uint16_t* resultBuffer;
	vector<uint8_t> channels;

public:
	AdcController(GPIO_TypeDef* gpio, const vector<uint8_t>& channels);
	void startAdc();

	int getChannelValue(int channel);
};

#endif /* ADCCONTROLLER_H_ */
