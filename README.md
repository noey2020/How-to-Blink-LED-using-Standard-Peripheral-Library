# How-to-Blink-LED-using-Standard-Peripheral-Library

Let's Blink an LED Using Standard Peripheral Library!	June 30, 2020

Earlier, I showed you how to toggle or blink an LED using ARM assembly. We went through
motions of meticulous register programming from dealing with absolute hex numbers, 
using mnenomics like assembly EQU and C defines, and reading/writing to registers in C
via 1UL << Bit_number_of_Register.

Let's do a new twist by using pre-written libraries to aid and save us time and effort
tweaking high-level C code. For starters, we'll choose the Standard Periperal Libraries.
I downloaded the STM32 SPL for STM32L1 from the ST website. After extracting, we have, 
at our disposal, an exhaustive set of header and C files.

We start with our very minimal C program having only a main function.

int main(void){

    return;
    
}

From experience, we know we have to enable the peripheral clock and the gpio. The first
step we need to do is to include these three header files.

#include "stm32l1xx.h"

#include "stm32l1xx_rcc.h"

#include "stm32l1xx_gpio.h"

Ok, let's examine what we have inside the gpio header. We see defines, function
prototypes, and typedef structs we can use. I list some them.

defines:

#define GPIO_Pin_0        ((uint16_t)0x0001)

#define GPIO_Pin_1        ((uint16_t)0x0002)

#define GPIO_Pin_2        ((uint16_t)0x0004)

...

...

#define GPIO_PinSource0   ((uint8_t)0x00)

#define GPIO_PinSource1   ((uint8_t)0x01)

#define GPIO_PinSource2   ((uint8_t)0x02)

...

...


function prototypes:

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


/*  Function used to set the GPIO configuration to the default reset state ****/

void GPIO_DeInit(GPIO_TypeDef* GPIOx);


/* Initialization and Configuration functions *********************************/

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);

void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);

void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


/* GPIO Read and Write functions **********************************************/

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);

void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* GPIO Alternate functions configuration functions ***************************/

void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF);

...

...

typedef struct

{
  uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
  
                                       This parameter can be any value of @ref GPIO_pins_define */
                                       

  GPIOMode_TypeDef GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
  
                                       This parameter can be a value of @ref GPIOMode_TypeDef */
                                       

  GPIOSpeed_TypeDef GPIO_Speed;   /*!< Specifies the speed for the selected pins.
  
                                       This parameter can be a value of @ref GPIOSpeed_TypeDef */
                                       

  GPIOOType_TypeDef GPIO_OType;   /*!< Specifies the operating output type for the selected pins.
  
                                       This parameter can be a value of @ref GPIOOType_TypeDef */

  GPIOPuPd_TypeDef GPIO_PuPd;     /*!< Specifies the operating Pull-up/Pull down for the selected pins.
  
                                       This parameter can be a value of @ref GPIOPuPd_TypeDef */
}GPIO_InitTypeDef;

...

...

... etc. etc. etc.

If we want to see the implementation details, we just inspect and study their respective
C files which are:

stm32l1xx_gpio.c

stm32l1xx_rcc.c

stm32l1xx.c

As an aside, I noticed many programmers tend to obfuscate or hide their code by including
defines and functions in many different programs. So, you really have to be diligent and
understand the call flow or even draw call graphs.

Do the same for the other header & C files.

#define GPIOB                 ((GPIO_TypeDef *) 0x40020400)

#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)

#define RCC                   ((RCC_TypeDef *) RCC_BASE)

#define RCC_AHBENR_GPIOBEN    ((uint32_t)0x00000002)

#define PERIPH_BASE           ((uint32_t)0x40000000)

#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

#define GPIOB_BASE            (AHBPERIPH_BASE + 0x0400)

...

...etc. etc. etc.

Now, this is where we piece all of these 
information together to blink our LED using Standard Peripheral Library.

Here's our first draft for blinky using STM32L1 Discovery kit:

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

Experiment Results from first Draft:

1) The order of includes should be #include "nc_stm32l1_gpio.h" then
   #include "stm32l1xx_rcc.h". Otherwise, you will get implicit declaration errors.
2) I had to uncomment line 71 #define STM32L1XX_MD of stm32l1xx.h. stm32l1xx.h is 
   different in every version so caveat.
3) I wrote my own bare nc_stm32l1_gpio.h and nc_stm32l1_gpio.c files. Reused, in other
   words, leveraged proven, mature code but maintain compatibility with HAL and SPL.
   See attached files.
4) Read and complied with GNU GPL v3 license.

5) The new,tested working blink_led_spl.c code is shown below.

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

Don't forget the newline and happy coding!

