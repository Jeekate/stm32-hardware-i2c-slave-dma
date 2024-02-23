
#include <string.h>
#include "i2c_slave.h"
#include "i2c.h"
//#include "sensor.h"
//#include "key.h"

typedef struct SLAVE_GVAR_ST{
	uint8_t addr;
	uint8_t slave_rx[SLAVE_BUF_LEN];
	uint8_t slave_tx[SLAVE_BUF_LEN];
	uint8_t dir;
	uint16_t rxlen;
} slave_gvar_st;

static slave_gvar_st slave_global = {0};


/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	uint16_t addr, rx_len;
	
	if (hi2c->Instance == I2C1)
	{
		/* write mem : partial data recieved */
		if ( __HAL_I2C_GET_FLAG(hi2c,  I2C_FLAG_DIR) == I2C_DIRECTION_TRANSMIT )
		{
			addr = slave_global.addr;
			rx_len = SLAVE_RX_BUF_LEN - __HAL_DMA_GET_COUNTER(hi2c->hdmarx) - 1;
			slave_global.rxlen = rx_len;
			memcpy(slave_global.slave_tx + addr, slave_global.slave_rx, rx_len);
		}
		/* read mem */
		else
		{
			/* nothing here */
		}
	}

	/* new recv */
	slave_start_dma();
}

/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection Master request Transfer Direction (Write/Read), value of @ref I2C_XFERDIRECTION
  * @param  AddrMatchCode Address Match Code
  * @retval None
  */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	uint16_t len, addr;
	
  if (hi2c->Instance == I2C1)
	{
		/* master transmit */
		if (TransferDirection == I2C_DIRECTION_TRANSMIT)
		{
			HAL_I2C_Slave_Seq_Receive_DMA(&hi2c1, (uint8_t*)&slave_global, SLAVE_RX_BUF_LEN, I2C_LAST_FRAME);
		}
		/* master receive */
		else
		{
			/* read mem */
			addr = slave_global.addr;
			len = SLAVE_TX_BUF_LEN - addr;
			HAL_I2C_Slave_Seq_Transmit_DMA(hi2c, &slave_global.slave_tx[addr], len, I2C_LAST_FRAME);
		}
	}
	
  UNUSED(AddrMatchCode);
}


/* write mem: all byte recieved */
void slave_dma_cb(void)
{
	if ((__HAL_I2C_GET_FLAG(&hi2c1,  I2C_FLAG_DIR) == I2C_DIRECTION_TRANSMIT) && 
			(slave_global.addr == 0))
	{
		memcpy(slave_global.slave_tx, slave_global.slave_rx, SLAVE_BUF_LEN);
	}
	slave_start_dma();
}

void slave_start_dma(void)
{
	HAL_I2C_EnableListen_IT(&hi2c1);
	
	HAL_I2C_Slave_Seq_Receive_DMA(&hi2c1, (uint8_t*)&slave_global, SLAVE_RX_BUF_LEN, I2C_FIRST_FRAME); 
}

uint8_t* slave_map_mem(uint8_t addr)
{
	return &slave_global.slave_tx[addr];
}

uint8_t slage_get_state(void)
{
	return (__HAL_I2C_GET_FLAG(&hi2c1, I2C_FLAG_BUSY) == SET);
}


