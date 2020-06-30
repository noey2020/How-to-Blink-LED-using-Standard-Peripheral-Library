#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#define GPIOB    ((GPIO_TypeDef *) GPIOB_BASE)

int main(void){

    GPIO_InitTypeDef GPIO_InitDef;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
 
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;
    //Initialize pins
    GPIO_Init(GPIOB, &GPIO_InitDef);
 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
 
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;
    //Initialize pins
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    while(1){
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
            GPIO_SetBits(GPIOB, GPIO_Pin_6);
        } else {
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);
        }
    }
}
