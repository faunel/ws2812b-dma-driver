#ifndef __WS2812B_DMA_DRIVER_H__
#define __WS2812B_DMA_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* Definition of TIM instance */
#define TIMx                             	TIM1

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE                  	__HAL_RCC_TIM1_CLK_ENABLE
#define TIMx_CLK_DISABLE                  	__HAL_RCC_TIM1_CLK_DISABLE
#define DMAx_CLK_ENABLE                  	__HAL_RCC_DMA2_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_GPIO_CLK_ENABLE    	        __HAL_RCC_GPIOE_CLK_ENABLE
#define TIMx_GPIO_PORT          	        GPIOE

#if WS2812B_LEDS_PIN == 1 // ok

#define TIMx_GPIO_PIN                	    GPIO_PIN_9
#define GPIO_AF_TIMx                     	GPIO_AF1_TIM1

/* Definition for TIMx channels */
#define TIMx_CHANNEL                        TIM_CHANNEL_1
#define TIM_DMA_HANDLE_ID                   TIM_DMA_ID_CC1

/* Definition for TIMx's DMA */
#define TIMx_CC1_DMA_CHANNEL             	DMA_CHANNEL_6
#define TIMx_CC1_DMA_STREAM               	DMA2_Stream1

/* Definition for DMAx's NVIC */
#define TIMx_DMA_IRQn                    	DMA2_Stream1_IRQn
#define TIMx_DMA_IRQHandler              	DMA2_Stream1_IRQHandler

#elif WS2812B_LEDS_PIN == 2

#define TIMx_GPIO_PIN                	    GPIO_PIN_11
#define GPIO_AF_TIMx                     	GPIO_AF1_TIM1

/* Definition for TIMx channels */
#define TIMx_CHANNEL                        TIM_CHANNEL_2
#define TIM_DMA_HANDLE_ID                   TIM_DMA_ID_CC2

/* Definition for TIMx's DMA */
#define TIMx_CC1_DMA_CHANNEL             	DMA_CHANNEL_6
#define TIMx_CC1_DMA_STREAM               	DMA2_Stream2

/* Definition for DMAx's NVIC */
#define TIMx_DMA_IRQn                    	DMA2_Stream2_IRQn
#define TIMx_DMA_IRQHandler              	DMA2_Stream2_IRQHandler

#elif WS2812B_LEDS_PIN == 3

#define TIMx_GPIO_PIN                	    GPIO_PIN_13
#define GPIO_AF_TIMx                     	GPIO_AF1_TIM1

/* Definition for TIMx channels */
#define TIMx_CHANNEL                        TIM_CHANNEL_3
#define TIM_DMA_HANDLE_ID                   TIM_DMA_ID_CC3

/* Definition for TIMx's DMA */
#define TIMx_CC1_DMA_CHANNEL             	DMA_CHANNEL_6
#define TIMx_CC1_DMA_STREAM               	DMA2_Stream6

/* Definition for DMAx's NVIC */
#define TIMx_DMA_IRQn                    	DMA2_Stream6_IRQn
#define TIMx_DMA_IRQHandler              	DMA2_Stream6_IRQHandler

#elif WS2812B_LEDS_PIN == 4

#define TIMx_GPIO_PIN                	    GPIO_PIN_14
#define GPIO_AF_TIMx                     	GPIO_AF1_TIM1

/* Definition for TIMx channels */
#define TIMx_CHANNEL                        TIM_CHANNEL_4
#define TIM_DMA_HANDLE_ID                   TIM_DMA_ID_CC4

/* Definition for TIMx's DMA */
#define TIMx_CC1_DMA_CHANNEL             	DMA_CHANNEL_6
#define TIMx_CC1_DMA_STREAM               	DMA2_Stream4

/* Definition for DMAx's NVIC */
#define TIMx_DMA_IRQn                    	DMA2_Stream4_IRQn
#define TIMx_DMA_IRQHandler              	DMA2_Stream4_IRQHandler

#endif

/* ws2812b defines */ 
#define WS2812B_FREQ			            (800000)        // it is fixed: WS2812 require 800kHz
#define TIMER_PRESCALER                     (1)
#define TIMER_CLOCK_FREQ	                (168000000)     // can be modified - multiples of 0.8MHz are suggested
#define WS2812B_TH1                         (151)           // 0.9us ->  18/25 * TIMER_PERIOD
#define WS2812B_TH0                         (59)            // 0.35us ->  7/25 * TIMER_PERIOD
#define TIMER_PERIOD		                (TIMER_CLOCK_FREQ / WS2812B_FREQ)
#define NUM_LEDS                            (WS2812B_LEDS_NUM)
#define WORDS_PER_LED                       (24)
#define RESET_SLOT                          (8)
#define LED_BUFFER_SIZE                     ((NUM_LEDS*WORDS_PER_LED)+RESET_SLOT)

typedef struct Color
{
	uint8_t r; // red
	uint8_t g; // green
	uint8_t b; // blue
}Color_t;

/* ws2812b-dma-driver API */ 

void ws2812b_init(void);
void drawFrame(void);
void clearAll(void);
void getPixelColor(int px_index, Color_t * px_color);
void setPixel_GRB(const Color_t * Color, int px_index);
void setAll_GRB(const Color_t * color);
void setRange_GRB(const Color_t * color, int start_px_index, int len);
void setColorBrightness(const Color_t * in, Color_t * out, float brightness);

/* ******************** */ 

void TIMx_DMA_IRQHandler(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif