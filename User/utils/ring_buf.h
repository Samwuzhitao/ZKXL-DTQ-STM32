/**
  ******************************************************************************
  * @file   	ringbuffer.h
  * @author  	sam.wu
  * @version 	V1.0.0.0
  * @date   	2016.8.26
  * @brief   	ringbuffer
  ******************************************************************************
  */
	
#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#include "stm32f10x.h"
#include "app_serial_cmd_process.h"
#include "rf_link_pro.h"

/* Private define ------------------------------------------------------------*/
/* Set parameter of budffer */
#define PRINT_BUF            0
#define SPI_RBUF             1
#define UART_RBUF            2

#define PACKETSIZE           (UART_NBUF+9)
#define SPI_RBUF_SIZE        (1024*5)
#define PRINT_RBUF_SIZE      (1024*10)
#define BUF_NUM              (3)


/* buffer status  */
#define BUF_EMPTY            1
#define BUF_USEING           2
#define BUF_FULL             3

/* the threshold of buffer */
#define USAGE_TATE_FULL      (100-PACKETSIZE*100/UART_RBUF_SIZE)
#define USAGE_TATE_EREMPTY   0

/* buffer operation */
#define OPERATIONWRITE       1
#define OPERATIONREAD        2

#define CLOSEIRQ()           __set_PRIMASK(1)        
#define OPENIRQ()            __set_PRIMASK(0)

/* Private functions ---------------------------------------------------------*/
uint8_t buffer_get_buffer_status( uint8_t sel );
uint8_t serial_ringbuffer_get_usage_rate(uint8_t sel);
int8_t  rf_read_data_from_buffer( uint8_t sel, rssi_rf_pack_t *pack );
void    rf_write_data_to_buffer( uint8_t sel, uint8_t *buf, uint8_t len );
void    print_write_data_to_buffer( char *str, uint8_t len );
uint8_t print_read_data_to_buffer( uint8_t *str ,uint8_t size);
#endif
