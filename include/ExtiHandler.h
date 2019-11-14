/*
 * ExtiHandler.h
 *
 *  Created on: Oct 20, 2019
 *      Author: chris
 */

#ifndef EXTIHANDLER_H_
#define EXTIHANDLER_H_

#include <vector>

#include "IoDriver.h"

using namespace std;

class ExtiHandler {
private:
	GPIO_TypeDef* gpio;
	vector<uint8_t> pins;

public:
	ExtiHandler(GPIO_TypeDef* gpio, std::vector<uint8_t>* const pins);

	void setupTrigger(GPIO_TypeDef* gpio);
};



#endif /* EXTIHANDLER_H_ */
