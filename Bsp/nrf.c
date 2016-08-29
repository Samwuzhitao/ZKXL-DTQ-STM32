/**
  ******************************************************************************
  * @file   	hal_nrf.c
  * @author  	Tian erjun
  * @version 	V1.0.0.0
  * @date   	2015.11.05
  * @brief   	hal function for nrf moulde
  ******************************************************************************
  */
 
#include "main.h"
#include "nrf.h"

#define NRF_DEBUG

#ifdef NRF_DEBUG
#define nrf_debug  printf   
#else  
#define nrf_debug(...)                    
#endif 

extern nrf_communication_t			nrf_communication;
extern uint8_t 					dtq_to_jsq_sequence;
extern uint8_t 					jsq_to_dtq_sequence;

void SPI_Init_NRF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	/* Configure SPI_MISO Pin */
	GPIO_InitStructure.GPIO_Pin   = SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

	/* Configure SPI_MOSI Pin */
	GPIO_InitStructure.GPIO_Pin   = SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);

	/* Configure SPI_SCK Pin */
	GPIO_InitStructure.GPIO_Pin   = SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

	/* Configure SPI_CSN Pin */								//CSN_1 配置
	GPIO_InitStructure.GPIO_Pin   = SPI_CSN_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_CSN_PORT, &GPIO_InitStructure);
	
	/* Configure SPI_CSN Pin */								//CSN_2 配置
	GPIO_InitStructure.GPIO_Pin   = SPI_CSN_PIN_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_CSN_PORT_2, &GPIO_InitStructure);

	/* Configure SPI_CE Pin */
	GPIO_InitStructure.GPIO_Pin   = SPI_CE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_CE_PORT, &GPIO_InitStructure);

	/* Configure SPI_IRQ Pin */
	GPIO_InitStructure.GPIO_Pin   = SPI_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_IRQ_PORT, &GPIO_InitStructure);

	/* SPI中断配置 */
	GPIO_EXTILineConfig(RFIRQ_PortSource, RFIRQ_PinSource);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_Line = EXTI_LINE_RFIRQ;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* SPI相关参数配置 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			//空闲为低	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		//第一个电平读取信号  模式0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		//不超过2M
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);							
	SPI_CSN_HIGH();		//片选拉高，禁止SPI
	SPI_CSN_HIGH_2();	//别忘，浪费我半天时间
}

/*******************************************************************************
  * @brief  双频点，一收一发
  * @param  None
  * @retval None
*******************************************************************************/
void NRF_Module_Set(void)
{
	SPI_Init_NRF();	//2.4G接收端改为改为51822,只需要SPI相关配置，
}


/******************************************************************************
  Function:my_nrf_transmit_start
  Description:
  Input:	data_buff：	   要发送的数组
			data_buff_len：要发送的数组长度
			nrf_data_type：发送数据类型，有效数据:NRF_DATA_IS_USEFUL 
										ACK		:NRF_DATA_IS_ACK
  Output:
  Return:
  Others:注意：通信方式限制，若向同一UID答题器下发数据，时间要间隔3S以上
******************************************************************************/
void my_nrf_transmit_start(uint8_t *data_buff, uint8_t data_buff_len,uint8_t nrf_data_type)
{

	if(nrf_data_type == NRF_DATA_IS_USEFUL)		//有效数据包，发送nrf_communication.transmit_buf内容
	{
		nrf_communication.transmit_ing_flag = true;
		nrf_communication.transmit_ok_flag = false;
		jsq_to_dtq_sequence++;
		nrf_communication.transmit_buf[0] = jsq_to_dtq_sequence;	
		memcpy((nrf_communication.transmit_buf + 1), nrf_communication.jsq_uid, 4);
		nrf_communication.transmit_buf[5] = NRF_DATA_IS_USEFUL;			
		nrf_communication.transmit_buf[6] = 0;
		memcpy((nrf_communication.transmit_buf + 10), data_buff, data_buff_len);	//有效数据从第10位开始放
		
		nrf_communication.transmit_len = NRF_TOTAL_DATA_LEN;	
		//uesb_nrf_write_tx_payload(nrf_communication.transmit_buf,nrf_communication.transmit_len);	//在定时器里发送，并判断是否发送成功
		TIM_Cmd(TIM3, ENABLE);  	
	}	
	else if(nrf_data_type == NRF_DATA_IS_ACK)	//ACK数据包，发送nrf_communication.software_ack_buf 内容
	{
		nrf_communication.software_ack_buf[0] = dtq_to_jsq_sequence;		
		memcpy((nrf_communication.software_ack_buf + 1), nrf_communication.jsq_uid, 4);
		nrf_communication.software_ack_buf[5] = NRF_DATA_IS_ACK;	
		
		nrf_communication.software_ack_len = NRF_TOTAL_DATA_LEN;	
		uesb_nrf_write_tx_payload(nrf_communication.software_ack_buf,nrf_communication.software_ack_len);	
	}
	else{;}
	

}


/******************************************************************************
  Function:my_nrf_transmit_tx_success_handler
  Description:向答题器发送数据后，收到答题器返回的软件ACK
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void my_nrf_transmit_tx_success_handler(void)
{
	nrf_debug("nrf_debug，发送成功，包号：%02X	\n",jsq_to_dtq_sequence);	
}


/******************************************************************************
  Function:my_nrf_transmit_tx_success_handler
  Description:向答题器发送数据后，达到最大重发次数后，未收到答题器返回的软件ACK
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void my_nrf_transmit_tx_failed_handler(void)
{
	nrf_debug("nrf_debug，发送失败，包号：%02X	\n",jsq_to_dtq_sequence);	
}


/******************************************************************************
  Function:my_nrf_transmit_tx_success_handler
  Description:收到答题器下发下来的数据
  Input:None
  Output:
  Return:
  Others:答题器下发的数据完整保存在rf_var.rx_buf（包括包头0x5A、包尾0xCA、XOR校验、有效数据等），根据需要自行处理
******************************************************************************/
void my_nrf_receive_success_handler(void)
{
	uint8_t i;
	nrf_debug("nrf_debug，收到答题器下发有效数据。打印如下：\n");
	for(i = 0; i < rf_var.rx_len; i++)		
	{
		nrf_debug("%02X ", rf_var.rx_buf[i]);
	}nrf_debug("\r\n");
	
}




/**
  * @}
  */
/**************************************END OF FILE****************************/


