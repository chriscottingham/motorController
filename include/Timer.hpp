/*
 * Timer.h
 *
 *  Created on: Jan 23, 2020
 *      Author: chris
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <functional>

using namespace std;

class Timer {
private:
	uint32_t tickCount = 0;
	uint32_t period;

	function<void (void)> callback;

public:

	Timer(uint32_t period, function<void (void)> callback);
	virtual ~Timer();

	void tick();
};

#endif /* TIMER_H_ */
