#include "stm32_ub_lis302dl.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "main.h"

int main(void){
  int16_t x,y,z;

//  SystemInit();
//  while(!(RCC->CR & RCC_CR_HSERDY)){} //proverka na zapusk occ
//  RCC -> CR |= RCC_CR_HSEON;
//  while(!(RCC->CR & RCC_CR_HSEON)){}

  USART_Configuration();
  USART2 -> BRR = (16000000/115200);
  UB_LIS302_Init(SCALE_8G);

  while(1) {
    UB_LIS302_Read(); // read LIS302
    x=LIS302.x_achse;
    y=LIS302.y_achse;
    z=LIS302.z_achse;
    int var =4;
    while(var < 40000) {
    	var++;
    }

    println("\n\r");
    splitAndSend(x);
    println(",");
    splitAndSend(y);
    println(",");
    splitAndSend(z);
  }
}

void splitAndSend(int16_t coordinate){
	sendData((coordinate & 0xFF00) >> 8);
	sendData(coordinate & 0xFF);
}

void USART_Configuration(void){
// sort out clocks
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Map USART2 to A.02
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	// Initialize USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	/* Configure USART */
	USART_Init(USART2, &USART_InitStructure);
	/* Enable the USART */
	USART_Cmd(USART2, ENABLE);
}

int size(char *ptr){
    int count = 0;
    while (*(ptr + count) != '\0'){
        ++count;
    }
    return count;
}

void sendData(int8_t ch){
  while(!(USART2->SR & USART_SR_TXE) | !(USART2->SR & USART_SR_TC));
  USART2->DR = ch;
}

void USART_PutChar(char ch){
  while(!(USART2->SR & USART_SR_TXE) | !(USART2->SR & USART_SR_TC));
  USART2->DR = ch;
}

int println(char *str){
	while(*str != 0){
	  USART_PutChar(*str);
	  str++;
	}
	return 0;
}

int clearArray(char *str){
	while(*str != 0){
		*str=0;
		str++;
	}
	return 0;
}