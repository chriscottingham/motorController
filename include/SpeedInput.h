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
	long maxRpm;
	AdcController* adcController;
	uint8_t adcValueIndex;

public:
	SpeedInput(uint8_t adcValueinde);

	void setAdcController(AdcController* adcController);
	void setMaxRpm(uint16_t maxRpm);
	void setAdcValuePointer(int* adcValuePointer);

	int getInputSpeed();

};


#endif /* SPEEDControl_H_ */
