/*
 * AdcController.h
 *
 *  Created on: Nov 28, 2019
 *      Author: chris
 */

#ifndef ADCCONTROLLER_H_
#define ADCCONTROLLER_H_

struct AdcState {
	int values[9];
};

class AdcController {
public:
	virtual ~AdcController(){};

	void enableChannel(int channel);
};

#endif /* ADCCONTROLLER_H_ */
