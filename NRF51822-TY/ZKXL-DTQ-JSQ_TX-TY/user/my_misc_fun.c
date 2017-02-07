

#include "my_misc_fun.h"


/******************************************************************************
  @函数:XOR_Cal
  @描述:
  @输入:* dat 异或的数组，及第几位开始
		length，数组中需要异或的长度
  @输出:数组异或的结果
  @调用:
******************************************************************************/
uint8_t XOR_Cal(uint8_t * dat,uint16_t length)
{
	uint8_t temp_xor;
	uint16_t i;

	temp_xor = *dat;
	for(i = 1;i < length; i++)
	{
		temp_xor = temp_xor ^ *(dat+i);
	}
	return temp_xor;
}

/******************************************************************************
  @函数:gpio_default_init
  @描述:51822的gpio配置为默认(省功耗)状态
  @输入:
  @输出:
  @调用:
******************************************************************************/
void gpio_default_init(void)
{
    uint32_t i = 0;
    for(i = 0; i< 32 ; ++i ) {
        NRF_GPIO->PIN_CNF[i] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                               | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                               | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                               | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos)
                               | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
    }
}

/******************************************************************************
  @函数:clocks_start
  @描述:开启外部16M时钟
  @输入:
  @输出:
  @调用:
******************************************************************************/
void clocks_start(void)
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
	
	
	 /* Start low frequency crystal oscillator for app_timer(used by bsp)*/
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }	
	
    NRF_CLOCK->EVENTS_DONE = 0;					//RTC校准
    NRF_CLOCK->TASKS_CAL = 1;
//    while (NRF_CLOCK->EVENTS_DONE == 0)
//    {
//        // Do nothing.
//    }
	
}


/******************************************************************************
  @函数:get_random_number
  @描述:获取0~255之间的随机数
  @输入:
  @输出:
  @调用:此函用于收到接收器下发数据后，随机延时0`255ms后再返回软件ACK，
		 减少多答题器同时回复ACK产生的数据冲突
******************************************************************************/
uint8_t get_random_number(void)
{
    NRF_RNG->TASKS_START = 1; // start the RNG peripheral.

	// Wait until the value ready event is generated.
	while (NRF_RNG->EVENTS_VALRDY == 0)
	{
		// Do nothing.
	}
	NRF_RNG->EVENTS_VALRDY = 0;		 // Clear the VALRDY EVENT.
	
	return (uint8_t)NRF_RNG->VALUE;
}

void parameters_init(void)
{
}


