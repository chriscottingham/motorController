
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <MotorDisplay.h>
#include "SpeedInput.h"
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

}

void SpeedInputTask(void* param) {

	//moving this outside of function makes screen flicker. Why?
	SpeedInput* speedInput;
	speedInput = &SpeedInput(GPIOA, 2);

	speedInput->setStateHolder((StateHolder<SpeedInputState>*) param);
	speedInput->run();
}

void MotorDisplayTask(void* param) {

	motorDisplay = &MotorDisplay();
	motorDisplay->setInitialization((MotorDisplayInitializer*) param);
	motorDisplay->runTask();
}

void RotaryEncoderTask(void* param) {

	encoder = &RotaryEncoder(GPIOA, &vector<uint8_t>({0, 1}));
	encoder->setEncoderStateHolder((StateHolder<EncoderState>*) param);
	encoder->run();
}


void init(void* param) {

	RtosQueueStateHolder<EncoderState> encoderStateHolder(1, EncoderState(0));

	xTaskCreate(RotaryEncoderTask, "RotaryEncoder", 500, &encoderStateHolder, 2, 0);

	RtosQueueStateHolder<SpeedInputState> speedStateHolder(1, SpeedInputState(1432));

	MotorDisplayInitializer displayInitializer;
	displayInitializer.encoderStateHolder = &encoderStateHolder;
	displayInitializer.speedInputStateHolder = &speedStateHolder;
	xTaskCreate(MotorDisplayTask, "MotorDisplay", 2000, &displayInitializer, 3, 0);

	xTaskCreate(SpeedInputTask, "SpeedInput", 200, &speedStateHolder, 4, 0);

	vTaskSuspend(xTaskGetCurrentTaskHandle());

	while (1) {
	}
}

//	__enable_irq();

	//Clock output
//	IoDriver::initPin(GPIOA, std::vector<uint8_t>{8}, GpioMode::altPushPullOutput);
//	RCC->CFGR |= 0x4000000;

//	ExtiHandler buttonHandler(GPIOB, std::vector<uint8_t> {10,11});
//	buttonHandler.setupTrigger(GPIOA);
int main(int argc, char* argv[]) {

	trace_puts("Hello ARM World!");

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	xTaskCreate(init, "init", 500, 0, 1, 0);
	vTaskStartScheduler();

	while (1) {
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
