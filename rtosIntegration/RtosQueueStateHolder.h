/*
 * QueueStateHolder.h
 *
 *  Created on: Nov 12, 2019
 *      Author: chris
 */

#ifndef QUEUESTATEHOLDER_H_
#define QUEUESTATEHOLDER_H_

#include "FreeRTOS.h"
#include "queue.h"

#include "StateHolder.h"

template<typename ValueType> class RtosQueueStateHolder: public StateHolder<ValueType> {

private:
	QueueHandle_t queueHandle;

public:
	RtosQueueStateHolder(QueueHandle_t queueHandle) :
			queueHandle(queueHandle) {
	}

	~RtosQueueStateHolder() {}

	void set(ValueType message) {

	}

	ValueType get() {

	}

	ValueType get(int millis) {

	}

};

#endif /* QUEUESTATEHOLDER_H_ */
