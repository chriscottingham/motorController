
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <MotorDisplay.h>
#include "SpeedControl.h"
#include "ExtiHandler.h"
#include "RotaryEncoder.h"
#include "RtosQueueStateHolder.h"

#define configASSERT_DEFINED 1
extern "C" {
	#include "FreeRTOSConfig.h"
	#include "freeRTOS.h"
	#include "projdefs.h"
	#include "portmacro.h"
	#include "StateHolder.h"
	#include "task.h"
	#include "queue.h"
}

#pragma GCC diagnostic push

MotorDisplay* motorDisplay;
RotaryEncoder* encoder;
SpeedControl* speedControl;

extern "C"
{
	void EXTI0_IRQHandler(void) {

		//	NVIC_ClearPendingIRQ(EXTI0_IRQn);

		EXTI->PR = 1;
		__ISB();

	}

	void I2C1_EV_IRQHandler(void) {
		motorDisplay->handleI2cInterrupt();
	}

	void I2C1_ER_IRQHandler(void) {
		motorDisplay->handleI2cError();
	}

	void DMA1_Channel6_IRQHandler(void) {
		motorDisplay->handleDma();
	}

	void MemManage_Handler(void) {

	}
}

void MotorDisplayTask(void* param) {
	MotorDisplay driver;
	motorDisplay = &driver;
	motorDisplay->setEncoderStateHolder((StateHolder<EncoderState>*) param);
	motorDisplay->runTask();
}

void RotaryEncoderTask(void* param) {
	RotaryEncoder rotaryEncoder(GPIOA, &vector<uint8_t>({0, 1}));
	encoder = &rotaryEncoder;
	encoder->setEncoderStateHolder((StateHolder<EncoderState>*) param);
	encoder->run();
}

void SpeedControlTask(void* param) {
	SpeedControl control(GPIOA, 2);
	speedControl = &control;
	speedControl->run();
}

void init(void* param) {

	QueueDefinition* handle = xQueueCreate(1, sizeof(EncoderState));
	RtosQueueStateHolder<EncoderState> encoderStateHolder(handle, EncoderState(0));

	TaskHandle_t encoderHandle;
	xTaskCreate(RotaryEncoderTask, "RotaryEncoder", 500, &encoderStateHolder, 2, &encoderHandle);

	TaskHandle_t displayHandle;
	xTaskCreate(MotorDisplayTask, "MotorDisplay", 2000, &encoderStateHolder, 3, &displayHandle);


	vTaskSuspend(xTaskGetCurrentTaskHandle());

	while (1) {
	}
}

int main(int argc, char* argv[]) {

	trace_puts("Hello ARM World!");

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

//	__enable_irq();

	//Clock output
//	IoDriver::initPin(GPIOA, std::vector<uint8_t>{8}, GpioMode::altPushPullOutput);
//	RCC->CFGR |= 0x4000000;

//	ExtiHandler buttonHandler(GPIOB, std::vector<uint8_t> {10,11});
//	buttonHandler.setupTrigger(GPIOA);

	xTaskCreate(init, "init", 500, 0, 1, 0);
	vTaskStartScheduler();

	while (1) {
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
