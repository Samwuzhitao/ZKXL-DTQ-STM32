/**
  ******************************************************************************
  * @file   	main.h
  * @author  	Tian erjun
  * @version 	V1.0.0.0
  * @date   	2015.11.05
  * @brief   	include all other header files
  ******************************************************************************
  */
#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "board.h"
#include "stm32f10x.h"
#include "define.h"
#include "whitelist.h"
#include "version.h"

#include "stm32f10x_it.h"
#include "mfrc500.h"
#include "m24sr_cmd.h"
#include "app_timer.h"
#include "eeprom.h"
#include "gpio.h"

#include "nrf.h"
#include "ringbuffer.h"


#include "rf_link_protocol.h"
#include "spi_link_protocol.h"

#endif //_MAIN_H_
/**
  * @}
  */
/**************************************END OF FILE****************************/

