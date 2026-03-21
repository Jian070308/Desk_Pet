#include "Function.h"

uint8_t buf1[20];
uint8_t buf2[20];
uint8_t buf3[20];
uint8_t buf4[20];
uint8_t buf5[20];
uint8_t tx_data[5];

void function(){
    // 1. 在进入循环前，定义两个局部变量记录时间戳
    uint32_t aht20_func_last_time = HAL_GetTick();
    uint32_t oled_func_last_time = HAL_GetTick();

	while(1){
        // 2. 获取当前系统运行时间
        uint32_t current_time = HAL_GetTick();

        // 3. 每 500 毫秒读取一次温湿度
        if(current_time - aht20_func_last_time >= 500) {
            AHT20_Read(&now.temperature, &now.humidity);
            aht20_func_last_time = current_time;
        }

        // 4. 每 50 毫秒刷新一次 OLED（频率可以自己微调）
        if(current_time - oled_func_last_time >= 50) {
            OLED_Show();
            oled_func_last_time = current_time;
        }


		if(tx_flag){
			tx_data[0]=now.happy;
			tx_data[1]=now.hungry;
			tx_data[2]=now.tired;
			EEPROM_PageWrite(&hi2c1,ADDRESS_Write,ADDRESS_Start,tx_data,3);
			HAL_Delay(5);
			tx_flag=0;
			timer_save=0;
		    uint8_t buffer[1];
		    buffer[0]=INITIALIZED_FLAG;
	    	EEPROM_PageWrite(&hi2c1,ADDRESS_Write,FIRST_BOOT_FLAG_ADDR, buffer,1);
		}

		switch(ble_flag){
			case '1':
				Play();
				ble_flag=0;
				break;
			case '2':
				Feed();
				ble_flag=0;
				break;
			case '3':
				Wash();
				ble_flag=0;
				break;
			case '4':
				sprintf((char *)buf1,"happy_value:%u\r\n",now.happy);
				HAL_UART_Transmit(&huart3, buf1, strlen((const char*)buf1), 1000);
				HAL_Delay(10);
				sprintf((char *)buf2,"hungry_value:%u\r\n",now.hungry);
				HAL_UART_Transmit(&huart3, buf2, strlen((const char*)buf2), 1000);
				HAL_Delay(10);
				sprintf((char *)buf3,"tired_value:%u\r\n",now.tired);
				HAL_UART_Transmit(&huart3, buf3 ,strlen((const char*)buf3), 1000);
				ble_flag=0;
				break;
			case '5':
				sprintf((char *)buf4,"temperature:%.1f\r\n",now.temperature);
				HAL_UART_Transmit(&huart3, buf4, strlen((const char*)buf4), 1000);
				sprintf((char *)buf5,"humidity:%.1f\r\n",now.humidity);
				HAL_UART_Transmit(&huart3, buf5, strlen((const char*)buf5), 1000);
				ble_flag=0;
				break;
		}

		int key=KEY_Scan();

		switch(key){
			case 0:
			  break;
			case 1:
			  Play();
			  break;
			case 2:
			  Feed();
			  break;
			case 3:
			  Wash();
			  break;
			case 4:
			  __HAL_TIM_SET_AUTORELOAD(&htim3,500);
			  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,500/5);
			  HAL_Delay(100);
			  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
			  return;
		}

		HAL_Delay(10); // 保留一个非常短的延时，防止while(1)跑得太死，让出一点点CPU切片
	}
}
