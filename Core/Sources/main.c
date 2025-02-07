#include "stm32f072xb.h" //включит "core_cm0.h"

int main() {
	*((volatile uint32_t*)RCC_BASE + 0x14) 		|= RCC_AHBENR_GPIOAEN;
	*((volatile uint32_t*)GPIOA_BASE + 0x00)	&= ~GPIO_MODER_MODER5;
	*((volatile uint32_t*)GPIOA_BASE + 0x00)	|= GPIO_MODER_MODER5_0;
	*((volatile uint32_t*)GPIOA_BASE + 0x18)	|= GPIO_BSRR_BS_5;


	return 0;
}
