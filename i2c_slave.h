#ifndef __I2C_SLAVE_H__
#define __I2C_SLAVE_H__



#include "stm32l0xx_hal.h"

#define SLAVE_BUF_LEN				(256)
#define SLAVE_TX_BUF_LEN		(SLAVE_BUF_LEN)
#define SLAVE_RX_BUF_LEN		(SLAVE_BUF_LEN+1)




void slave_dma_cb(void);
uint8_t* slave_map_mem(uint8_t addr);
uint8_t slage_get_state(void);
void slave_start_dma(void);

#endif

