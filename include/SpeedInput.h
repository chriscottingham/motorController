/*
 * SpeedControl.h
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#ifndef SPEEDControl_H_
#define SPEEDControl_H_

#include "AdcController.h"

class SpeedInput {
private:
	int* adcValuePointer;
	long maxRpm;

public:
	void setMaxRpm(uint16_t maxRpm);
	void setAdcValuePointer(int* adcValuePointer);

	int getInputSpeed();

};


#endif /* SPEEDControl_H_ */
