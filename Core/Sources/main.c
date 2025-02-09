#include "stm32f072xb.h" //включит "core_cm0.h"

int main() {
	// *((volatile uint32_t*)(RCC_BASE + 0x14)) 	|= RCC_AHBENR_GPIOAEN;
	// *((volatile uint32_t*)(GPIOA_BASE + 0x00))	&= ~GPIO_MODER_MODER5;
	// *((volatile uint32_t*)(GPIOA_BASE + 0x00))	|= GPIO_MODER_MODER5_0;
	// *((volatile uint32_t*)(GPIOA_BASE + 0x18))	|= GPIO_BSRR_BS_5;

	//включаем LD
	RCC->AHBENR 	|= RCC_AHBENR_GPIOAEN; //включаем тактирование GPIOA (RM0091:47,136,149)
	GPIOA->MODER	&= ~ GPIO_MODER_MODER5_Msk; //чистим MODER5 (LD на GPIOA5) (RM0091:164,158)
	GPIOA->MODER	|= GPIO_MODER_MODER5_0; //и устанавливаем в "01: General purpose output mode"
	GPIOA->BSRR		|= GPIO_BSRR_BS_5; //устанавливаем порт в 1 (зажигаем LD)

	return 0;
}
