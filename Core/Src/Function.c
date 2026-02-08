#include "Function.h"

uint8_t buf1[20];
uint8_t buf2[20];
uint8_t buf3[20];
uint8_t buf4[20];
uint8_t buf5[20];
uint8_t tx_data[5];

void function(){
	while(1){
		AHT20_Read(&now.temperature,&now.humidity);
		OLED_Show();

		if(tx_flag){
			tx_data[0]=now.happy;
			tx_data[1]=now.hungry;
			tx_data[2]=now.tired;
			EEPROM_PageWrite(&hi2c1,ADDRESS_Write,ADDRESS_Start,tx_data,3);
			tx_flag=0;
			timer_save=0;
	        // 成功存储数据后，立即写入已初始化标志，防止下次再被判断为首次
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

		HAL_Delay(50);
	}
}
