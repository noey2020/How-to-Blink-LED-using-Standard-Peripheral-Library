#include "stm32l1xx.h"
#include "nc_stm32l1_gpio.h"
//#include "stm32l1xx_rcc.h"

int main(void){
    GPIO_InitTypeDef GPIO_InitDef;
    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;	//RCC->AHBENR	|= 0x00000002;

	  GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;
    //Initialize pins
    GPIO_Init(GPIOB, &GPIO_InitDef);
 
    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;	//RCC->AHBENR	|= 0x00000001;
 
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;
    //Initialize pins
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    while(1){
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
            GPIO_SetBits(GPIOB, GPIO_Pin_6);
        } else{
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);
        }
    }  
}
