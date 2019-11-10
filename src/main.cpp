/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "diag/Trace.h"

#include "BlinkLed.h"
#include "LedDisplayDriver.h"
#include "ExtiHandler.h"

#define configASSERT_DEFINED 1
extern "C" {
	#include "FreeRTOSConfig.h"
	#include "projdefs.h"
	#include "portmacro.h"
	#include "freeRTOS.h"
}
	#include "task.h"

#pragma GCC diagnostic push

LedDisplayDriver displayDriver;

extern "C"
{
	void EXTI0_IRQHandler(void) {

		//	NVIC_ClearPendingIRQ(EXTI0_IRQn);

		trace_puts("Exti0 triggered");
		EXTI->PR = 1;
		__ISB();

		displayDriver.printHello();
	}

	void I2C1_EV_IRQHandler(void) {
		displayDriver.handleI2cInterrupt();
	}

	void I2C1_ER_IRQHandler(void) {
		displayDriver.handleI2cError();
	}

	void DMA1_Channel6_IRQHandler(void) {
		displayDriver.dma6Handler();
	}
}

int main(int argc, char* argv[]) {

	trace_puts("Hello ARM World!");

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

//	__enable_irq();

//	IoDriver::initPin(GPIOA, std::vector<uint8_t>{8}, GpioMode::altPushPullOutput);
//	RCC->CFGR |= 0x4000000;

	uint32_t seconds = 0;

	IoDriver::initPin(GPIOB, std::vector<uint8_t> {6, 7}, GpioMode::pushPullOutput);
	GPIOB->ODR &= ~(GPIO_ODR_ODR6 | GPIO_ODR_ODR6);

	IoDriver::initPin(GPIOB, std::vector<uint8_t> {10,11}, GpioMode::pushPullOutput);
	GPIOB->ODR |= 0xc00;

	ExtiHandler buttonHandler;
	buttonHandler.setupTrigger(GPIOA);

//	displayDriver.initDma();
//	displayDriver.initI2c();

	TaskHandle_t taskHandle;
	xTaskCreate(LedDisplayDriverTask, "LedDisplayDriver", 20, &displayDriver, 1, &taskHandle);
	vTaskStartScheduler();

	bool printedI2cStart = false;

	while (1) {
//		displayDriver.handleInterrupt();

//		if (!printedI2cStart) {
//			if (I2C1->SR1 & 1) {
//				trace_puts("got start condition");
//				printedI2cStart = true;
//			}
//		}
//		char cmar[10];
//		sprintf(cmar, "0x%08X", DMA1_Channel6->CMAR);
//		trace_puts(cmar);

//      blinkLed.turnOn();
//      displayDriver.writeParallelPort(0xa5, 1);
//      timer.sleep(seconds == 0 ? Timer::FREQUENCY_HZ : BLINK_ON_TICKS);

//      displayDriver.writeParallelPort(0x5a, 1);
//      blinkLed.turnOff();
//      timer.sleep(BLINK_OFF_TICKS);

//      ++seconds;

		// Count seconds on the trace device.
//      trace_printf("Second %u\n", seconds);
	}
	// Infinite loop, never return.
}




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
