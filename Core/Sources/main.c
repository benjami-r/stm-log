#include "stm32f072xb.h" //включит "core_cm0.h"
#include "m_periphery_GPIO.h"

int main() {
	m_GPIOx_ENABLE(A);
	m_GPIOxpin_CONFIG(A, 5, mee_GPIO_MODER_GENERAL, mee_GPIO_OTYPER_PUSHPULL, mee_GPIO_OSPEEDR_LOW, mee_GPIO_PUPDR_NOPUPD);
	m_GPIOxpin_SET(A, 5)

	return 0;
}



