#include "EEPROM.h"

HAL_StatusTypeDef EEPROM_ReadBuffer(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                    uint8_t start_addr, uint8_t *pData, uint16_t size)
{
    HAL_StatusTypeDef status;

    // 先定位地址
    status = HAL_I2C_Master_Transmit(hi2c, device_addr << 1, &start_addr, 1, 1000);
    if (status != HAL_OK) return status;

    // 再读数据
    status = HAL_I2C_Master_Receive(hi2c, (device_addr << 1) | 0x01, pData, size, 1000);
    return status;
}

HAL_StatusTypeDef EEPROM_PageWrite(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                   uint8_t start_addr, uint8_t *pData, uint8_t len)
{
    uint8_t buffer[32 + 1];  // 最大32字节数据 + 1字节地址
    buffer[0] = start_addr;
    memcpy(buffer + 1, pData, len);

    return HAL_I2C_Master_Transmit(hi2c, device_addr << 1, buffer, len + 1, 1000);
}

// 判断是否是首次开机
// 返回值：1表示首次开机，0表示非首次开机
uint8_t IsFirstBoot() {
    uint8_t flag_value;  // 用于存储读取到的标志值


    // 1. 读取标志地址的值
    HAL_StatusTypeDef status = EEPROM_ReadBuffer(&hi2c1,ADDRESS_Write,FIRST_BOOT_FLAG_ADDR, &flag_value,1);

    // 2. 如果读取失败，按首次开机处理（保守策略）
    if (status != HAL_OK) {
        return 1;  // 返回1，表示首次开机
    }

    // 3. 判断标志值
    if (flag_value == FIRST_BOOT_FLAG) {

        return 1;  // 返回1，表示首次开机
    }

    return 0;  // 返回0，表示非首次开机
}







