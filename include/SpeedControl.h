/*
 * SpeedControl.h
 *
 *  Created on: Nov 15, 2019
 *      Author: chris
 */

#ifndef SPEEDControl_H_
#define SPEEDControl_H_

#include <vector>

#include "IoDriver.h"

using namespace::std;

class SpeedControl {
private:
	GPIO_TypeDef* gpio;
	uint8_t inputPin;

public:
	SpeedControl(GPIO_TypeDef* gpio, uint8_t inputPin);
	void run();
};



#endif /* SPEEDControl_H_ */
