#include "ws2812b-dma-driver.h"

TIM_HandleTypeDef htimx;
DMA_HandleTypeDef hdma_timx_chn;
static uint16_t * ws2812b_data;

void Error_Handler(void){}

static void MX_TIMx_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htimx.Instance = TIMx;
  htimx.Init.Prescaler = (uint32_t)TIMER_PRESCALER-1;
  htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx.Init.Period = (uint32_t)TIMER_PERIOD - 1;
  htimx.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htimx.Init.RepetitionCounter = 0;

//   htimx.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htimx) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htimx, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htimx) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htimx, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState=TIM_OCIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIMx_CHANNEL) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htimx);
}

static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  DMAx_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIMx_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIMx_DMA_IRQn);

}

void ws2812b_init(void)
{
    ws2812b_data = (uint16_t *)calloc(LED_BUFFER_SIZE, sizeof(uint16_t)); 
    assert_param(data);
    __disable_irq();
    MX_DMA_Init();
    MX_TIMx_Init();
    __enable_irq();
}

void drawFrame(void)
{
    __disable_irq();
	HAL_TIM_PWM_Start_DMA(&htimx, TIMx_CHANNEL, (uint32_t *)ws2812b_data, LED_BUFFER_SIZE);
    __enable_irq();
}

void setPixel_GRB(const Color_t * color, int px_index)
{
	int index = px_index * WORDS_PER_LED;
    for(int i=0;i<8;i++)
    {
        // high bit send at first
    	ws2812b_data[index+i] = (color->g >> (7-i)) & 1 ? WS2812B_TH1 : WS2812B_TH0;
    	ws2812b_data[index+i+8] = (color->r >> (7-i)) & 1 ? WS2812B_TH1 : WS2812B_TH0;
    	ws2812b_data[index+i+16] = (color->b >> (7-i)) & 1 ? WS2812B_TH1 : WS2812B_TH0;
    }
}

void setAll_GRB(const Color_t * color)
{
    for(int i=0;i<NUM_LEDS;i++)
    {
    	setPixel_GRB(color, i);
    }
}

void getPixelColor(int px_index, Color_t * px_color){
	int index = px_index * WORDS_PER_LED;
	uint32_t color = 0;
    for(int i=0;i<8;i++)
    {
    	px_color->g |= (ws2812b_data[index+i] == WS2812B_TH1 ? 1 : 0) << (7-i);
    	px_color->r |= (ws2812b_data[index+i+8] == WS2812B_TH1 ? 1 : 0) << (7-i);
    	px_color->b |= (ws2812b_data[index+i+16] == WS2812B_TH1 ? 1 : 0) << (7-i);
    }
}

void setRange_GRB(const Color_t * color, int start_px_index, int len)
{
    for(int i=start_px_index;i<start_px_index+len;i++)
    {
    	setPixel_GRB(color, i);
    }
}

void clearAll(void)
{
	for(int i=0;i<(LED_BUFFER_SIZE-RESET_SLOT);i++)
		ws2812b_data[i] = WS2812B_TH0;
}

void TIMx_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_timx_chn);
}
