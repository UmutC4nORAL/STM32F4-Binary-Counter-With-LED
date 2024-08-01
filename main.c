#include "stm32f4xx.h"                  // Device header
static volatile unsigned int counter = 0;
void EXTI0_IRQHandler(void);

void EXTI0_IRQHandler(void){
	EXTI->PR |= EXTI_PR_PR0; // set 0. bit to 1 of PR register, clean
	counter++;
	GPIOD->ODR &= ~(0XFU<<12); // clean bit 12-15 of ODR register
	GPIOD->ODR |= (counter & 0xFU) <<12; // set bit 12-15 to counter value
}

static void button_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // activate clock signal of A port
	GPIOA->MODER &= ~GPIO_MODER_MODER0; // set MODER register pin 0 to 0b00
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // activate clock signal of SYSCFG system configuration controller 
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0; // set pin 0 of SYSCFG_EXTICR1 register to 0b0000 
	EXTI->IMR |= EXTI_IMR_IM0; // set 0. bit to zero of IMR register
	EXTI->RTSR |= EXTI_RTSR_TR0; // interrupt works with button click, set 0. bit to 0 of RTSR register
	NVIC_EnableIRQ(EXTI0_IRQn); // activate EXTI0 interrupt
}

static void led_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // activate clock signal of D port
	GPIOD->MODER |= GPIO_MODER_MODER15_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER12_0; // set MODER registers pin 12 - 15 to 0b01
}

int main(){
	led_init(); // call led_init function
	button_init(); // call button_init function
	while(1); // infinite loop
} 
