#include "IoDriver.h"

void IoDriver::initPin(GPIO_TypeDef* const port, std::vector<uint8_t> const &pins,  GpioMode&& mode) {

	if (port == GPIOA) {
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	} else if (port == GPIOB) {
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	}
	if (mode.isAlternate()) {
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	}

	for (uint8_t pin : pins) {

		if (pin < 8) {
			port->CRL &= ~(0x0f << (4 * pin));
			port->CRL |= mode.getValue() << (4 * pin);
		} else {
			port->CRH &= ~(0x0f << (4 * (pin - 8)));
			port->CRH |= mode.getValue() << (4 * (pin - 8));
		}
	}

}
