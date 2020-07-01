/* Copyright (C) 2020, Noel Cruz

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>. */
	
#include "nc_stm32l1_gpio.h"

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;
  uint32_t tmpreg = 0x00;
  
  /* -------------------------Configure the port pins---------------- */
  /*-- GPIO Mode Configuration --*/
  for (pinpos = 0x00; pinpos < 0x10; pinpos++)
  {
    pos = ((uint32_t)0x01) << pinpos;

    /* Get the port pins position */
    currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

    if (currentpin == pos)
    {
      /* Use temporary variable to update MODER register configuration, to avoid 
         unexpected transition in the GPIO pin configuration. */
      tmpreg = GPIOx->MODER;
      tmpreg &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
      tmpreg |= (((uint32_t)GPIO_InitStruct->GPIO_Mode) << (pinpos * 2));
      GPIOx->MODER = tmpreg;

      if ((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF))
      {
        /* Use temporary variable to update OSPEEDR register configuration, to avoid 
          unexpected transition in the GPIO pin configuration. */
        tmpreg = GPIOx->OSPEEDR;
        tmpreg &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2));
        tmpreg |= ((uint32_t)(GPIO_InitStruct->GPIO_Speed) << (pinpos * 2));
        GPIOx->OSPEEDR = tmpreg;

         /* Use temporary variable to update OTYPER register configuration, to avoid 
          unexpected transition in the GPIO pin configuration. */
        tmpreg = GPIOx->OTYPER;
        tmpreg &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)pinpos));
        tmpreg |= (uint16_t)(((uint16_t)GPIO_InitStruct->GPIO_OType) << ((uint16_t)pinpos));
        GPIOx->OTYPER = tmpreg;
      }

      /* Use temporary variable to update PUPDR register configuration, to avoid 
         unexpected transition in the GPIO pin configuration. */
      tmpreg = GPIOx->PUPDR;
      tmpreg &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
      tmpreg |= (((uint32_t)GPIO_InitStruct->GPIO_PuPd) << (pinpos * 2));
      GPIOx->PUPDR = tmpreg;
    }
  }
}

/** @defgroup GPIO_Group2 GPIO Read and Write
 *  @brief   GPIO Read and Write
 *
@verbatim   
 ===============================================================================
                      ##### GPIO Read and Write #####
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A..H) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to read.
  *   This parameter can be GPIO_Pin_x where x can be (0..15).
  * @retval The input port pin value.
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;

  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->BSRRL = GPIO_Pin;
}

void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->BSRRH = GPIO_Pin;
}
