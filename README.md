# STM32硬件i2c从机DMA
[博客地址](https://blog.csdn.net/u012198579/article/details/136258360)
#### 介绍
基于HAL库函数的STM32单片机I2C从机代码，DMA（Seq）方式通信。

#### 软件架构
主要基于两个回调函数：

`void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)`

当接受/发送数据时收到STOP/NACK信号时被调用，可以认为此时主机以及写入/读取足够字节，本次传输结束


`void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)`

当主机发数据不满DMA计数时，进入这个回调函数，需要我们字节计算收到的字节数。并开启下一轮通信。 



工程中有4个逻辑分析仪文件，有linux下i2c-tool工具命令结果，另一个单片机当作主机读写从机的I2C波形图
当检测到总线上有本机地址时调用，判断数据方向后调用相关API
注意此时 _**I2C_LAST_FRAME**_ 参数，该参数导致无法进入I2C_DMASlaveReceiveCplt（初始化监听时注册）


#### 使用说明

1.  结构体的addr可以改成uint16_t以支持16位寄存器地址模式
2.  初始化时调用slave_start_dma即可
3.  目前测试暂未出现总线错误情况，若主机导致总线总裁失败，请查阅其他资料


