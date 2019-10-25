/*
 * ExtiHandler.h
 *
 *  Created on: Oct 20, 2019
 *      Author: chris
 */

#ifndef EXTIHANDLER_H_
#define EXTIHANDLER_H_

#include <vector>

#include "stm32f10x.h"

#include "GpioMode.h"
#include "IoDriver.h"

class ExtiHandler {

public:

	void setupTrigger(GPIO_TypeDef* gpio);
};



#endif /* EXTIHANDLER_H_ */
