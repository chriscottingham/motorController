/*
 * Timer.cpp
 *
 *  Created on: Jan 23, 2020
 *      Author: chris
 */

#include <Timer.hpp>

Timer::Timer(uint32_t period, function<void (void)> callback) : period(period), callback(callback) {
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::tick() {
	tickCount += 1;
	if (tickCount >= period) {
		callback();
		tickCount = 0;
	}
}
