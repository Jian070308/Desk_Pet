#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#define ADDRESS_Write 0x50
#define ADDRESS_Start 0x01

// 首次开机标志的存储地址（选择一个固定地址，如0x00）
#define FIRST_BOOT_FLAG_ADDR 0x00
// 首次开机的标志值（选择EEPROM擦除后的默认值，通常是0xFF）
#define FIRST_BOOT_FLAG 0x01
// 已初始化的标志值（可以设为0x00或其他非0xFF的值）
#define INITIALIZED_FLAG 0x00


#include "main.h"
#include "string.h"

HAL_StatusTypeDef EEPROM_ReadBuffer(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                    uint8_t start_addr, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef EEPROM_PageWrite(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                   uint8_t start_addr, uint8_t *pData, uint8_t len);
uint8_t IsFirstBoot();

#endif /* INC_EEPROM_H_ */
