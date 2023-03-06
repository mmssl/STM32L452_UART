#include "stm32l4xx.h"                  // Device header

void GPIO_Config (void);
void initClockHSI (void);
void delay (uint32_t time);
void UART2_Config (void);
void UART2_SendChar (uint8_t c);
uint8_t UART2_GetChar (void);



void initClockHSI (void)	
{                           
	RCC->CR |= RCC_CR_HSION; 
	while(!(RCC->CR & RCC_CR_HSIRDY)){}
	
	RCC->CFGR &= ~RCC_CFGR_SW;
			
	RCC->CFGR |= RCC_CFGR_SW_HSI;	
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI){}	
}

void GPIO_Config (void)
{

	/* 1. Enable the GPIO CLOCK 
		 2. Set the Pin as OUT
	   3. Configure the OUTPUT MODE */
		
	// 1. Enable the GPIO CLOCK
	
	
	RCC->AHB2ENR |= (1<<1); // RCC_AHB2ENR_GPIOBEN; (1<<1);

	/*LED*/
	GPIOB->MODER &= ~(0xffffffff);
	GPIOB->MODER |= 0x4000000;
	
	// 3. Configure the OUTPUT MODE
	
	GPIOB->OTYPER &= ~GPIO_OTYPER_ODR_13;
	GPIOB->OSPEEDR = (1<<27);
}
volatile uint16_t  uartdiv;
void UART2_Config (void)
{
	RCC->APB1ENR1 |= (1<<17); //UART2 enable
	RCC->AHB2ENR |= (1<<0);  // GPIOA enable
	RCC->CCIPR |= (0x2)<<2;
 // GPIOA->MODER &= ~(0x3<<4) & (0x3<<6); // Reset moder register
	GPIOA->MODER |= (0x2<<4) | (0x2<<6); 
	
	GPIOA->OSPEEDR &= ~(0xff);
	GPIOA->OSPEEDR |= (0xf0);
	
	GPIOA->AFR[0] &= ~(0xfffffffff);
	GPIOA->AFR[0] |= (7<<8) | (7<<12);
	
	
	uartdiv = 16000000 / 9600;
	USART2->CR1 &= ~(0xffffffff);
	USART2->CR1 &= ~(1<<12) & ~(1<<28);
	//USART2->CR2 &= ~(0xfffffffff);
//	USART2->CR2 &= ~(0x3000);
	USART2->BRR &= ~(0xfffffffff);
	USART2->BRR = uartdiv;
	USART2->CR1 |= (1<<0) | (1<<2) | (1<<3);

}
int a;
void UART2_SendChar (uint8_t c)
{


while(!(USART2->ISR & (1<<7))){}
	USART2->TDR = c; //load the data into TDR register
	a = USART2->TDR;
}

uint8_t UART2_GetChar (void)
{

	uint8_t temp;
	
	while(!(USART2->ISR & (1<<5)));
	temp = USART2->RDR;
	return temp;

}



void delay (uint32_t time)
{
	while(time--){}
}

int main()
{

	initClockHSI();
	GPIO_Config();
	UART2_Config();


while(1)
{
	UART2_SendChar ('D');
	delay(9999);
//	GPIOB->ODR ^= (1<<13);
	
//	uint8_t data = UART2_GetChar();
//	UART2_SendChar(data);

}
return 0;
}
