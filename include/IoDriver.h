/*
 * IoDriver.h
 *
 *  Created on: Oct 16, 2019
 *      Author: chris
 */

#ifndef IODRIVER_H_
#define IODRIVER_H_

#include <vector>

#include "stm32f10x.h"
#include "GpioMode.h"

using namespace std;

class IoDriver {
public:
	static void initPin(GPIO_TypeDef * const port, const vector<uint8_t>& pins, GpioMode&& mode);

};

#endif /* IODRIVER_H_ */
