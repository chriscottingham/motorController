#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic push

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <MotorDisplay.h>
#include "SpeedInput.h"
#include "ExtiHandler.h"
#include "RotaryEncoder.h"
#include "RtosQueueStateHolder.h"
#include "PwmControl.h"

#define configASSERT_DEFINED 1

//incommenting this makes display flash
//long MAX_MOTOR_RPM = 3600;

extern "C" {
	#include "FreeRTOSConfig.h"
	#include "freeRTOS.h"
	#include "projdefs.h"
	#include "portmacro.h"
	#include "StateHolder.h"
	#include "task.h"
	#include "queue.h"
}


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

void MotorDisplayTask(void* param) {

	motorDisplay = &MotorDisplay();
	motorDisplay->setInitialization((MotorDisplayInitializer*) param);
	motorDisplay->runTask();
}

void RotaryEncoderTask(void* param) {

	encoder = &RotaryEncoder(GPIOA, &vector<uint8_t>({0, 1}));
	encoder->setEncoderStateHolder((StateHolder<RotationState>*) param);
	encoder->run();
}

void SpeedInputTask(void* param) {

	//moving this outside of function makes screen flicker. Why?
	SpeedInput* speedInput;
	speedInput = &SpeedInput(GPIOA, 2);
	speedInput->setMaxRpm(3600);

	speedInput->setStateHolder((StateHolder<RotationState>*) param);
	speedInput->run();
}

void PwmControlTask(void* param) {

	PwmControlInitializer* initializer = (PwmControlInitializer*) param;
	PwmControl pwm(GPIOA, 6);
	pwm.setMaxMotorRpm(3600);
	pwm.setCurrentSpeedHolder(initializer->currentSpeed);
	pwm.setDesiredSpeedHolder(initializer->desiredSpeed);
	pwm.run();
}


void init(void* param) {

//	for (long i=0; i<1000000; i++);

	RtosQueueStateHolder<RotationState> encoderStateHolder(1, RotationState(0));

	xTaskCreate(RotaryEncoderTask, "RotaryEncoder", 500, &encoderStateHolder, 8, 0);

	RtosQueueStateHolder<RotationState> speedStateHolder(1, RotationState(1432));

	MotorDisplayInitializer displayInitializer;
	displayInitializer.encoderStateHolder = &encoderStateHolder;
	displayInitializer.speedInputStateHolder = &speedStateHolder;
	xTaskCreate(MotorDisplayTask, "MotorDisplay", 2000, &displayInitializer, 1, 0);

	xTaskCreate(SpeedInputTask, "SpeedInput", 200, &speedStateHolder, 8, 0);

	PwmControlInitializer pwmInitializer;
	pwmInitializer.currentSpeed = &encoderStateHolder;
	pwmInitializer.desiredSpeed = &speedStateHolder;
	xTaskCreate(PwmControlTask, "PwmControl", 200, &pwmInitializer, 8, 0);

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
