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

	uint32_t lowOrMask = 0;
	uint32_t lowAndMask = 0xffffffff;

	uint32_t highOrMask = 0;
	uint32_t highAndMask = 0xffffffff;

	for (uint8_t pin : pins) {

		if (pin < 8) {
			lowAndMask &= ~(0x0f << (4 * pin));
			lowOrMask |= mode.getValue() << (4 * pin);
		} else {
			highAndMask &= ~(0x0f << 4 * (pin - 8));
			highOrMask |= mode.getValue() << (4 * (pin - 8));
		}
	}

	port->CRL &= lowAndMask;
	port->CRL |= lowOrMask;

	port->CRH &= highAndMask;
	port->CRH |= highOrMask;
}
