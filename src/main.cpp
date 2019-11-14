
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <MotorDisplay.h>
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

MotorDisplay displayDriver;

extern "C"
{
	void EXTI0_IRQHandler(void) {

		//	NVIC_ClearPendingIRQ(EXTI0_IRQn);

		EXTI->PR = 1;
		__ISB();

	}

	void I2C1_EV_IRQHandler(void) {
		displayDriver.handleI2cInterrupt();
	}

	void I2C1_ER_IRQHandler(void) {
		displayDriver.handleI2cError();
	}

	void DMA1_Channel6_IRQHandler(void) {
		displayDriver.handleDma();
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


	TaskHandle_t displayHandle;
	xTaskCreate(MotorDisplayTask, "LedDisplayDriver", 200, &displayDriver, 2, &displayHandle);

	RotaryEncoder rotaryEncoder(GPIOA, &vector<uint8_t>({0, 1}));

	TaskHandle_t encoderHandle;
	xTaskCreate(RotaryEncoderTask, "RotaryEncoder", 200, &rotaryEncoder, 2, &encoderHandle);

	vTaskStartScheduler();

	bool printedI2cStart = false;

	while (1) {
	}
}




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
