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
	QueueDefinition * queueHandle;
	ValueType value;

public:
	RtosQueueStateHolder(QueueDefinition * queueHandle, ValueType value) :
			queueHandle(queueHandle), value(value) {
	}

	virtual ~RtosQueueStateHolder() {
	}

	void set(ValueType message) {
		xQueueOverwrite(queueHandle, &message);
	}

	ValueType get() {
		xQueueReceive(queueHandle, &value, 0);
		return value;
	}

	ValueType get(int millis) {

	}

};

#endif /* QUEUESTATEHOLDER_H_ */
