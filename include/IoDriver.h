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


class IoDriver {
public:
	static void initPin(GPIO_TypeDef * const port, std::vector<uint8_t> const &pins, GpioMode&& mode);

};

#endif /* IODRIVER_H_ */
