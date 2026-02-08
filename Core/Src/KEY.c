#include "KEY.h"

#define KEY_DEBOUNCE_TIME 20

extern TIM_HandleTypeDef htim3;

extern char message[20];

int KEY_Scan(){
    static uint32_t last_time = 0;
    uint32_t current_time = HAL_GetTick();

    // 限制扫描频率
    if(current_time - last_time < 5) return 0;
    last_time = current_time;

    // 检测按键状态
    uint8_t current_key = 0;
    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET){
        current_key = 1;
    }
    else if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET){
        current_key = 2;
    }
    else if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET){
        current_key = 3;
    }
    else if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET){
        current_key = 4;
    }

    // 简单的消抖逻辑
    static uint32_t press_time = 0;
    static uint8_t last_key = 0;

    if(current_key == 0){
        // 没有按键按下，重置状态
        last_key = 0;
        return 0;
    }

    if(current_key != last_key){
        // 检测到新的按键，开始计时
        last_key = current_key;
        press_time = current_time;
        return 0;  // 第一次检测到，不返回，等待消抖
    }
    else{
        // 同一个按键持续按下
        if(current_time - press_time > KEY_DEBOUNCE_TIME){
            // 消抖完成，返回键值
            last_key = 0;  // 重置，防止重复触发
            return current_key;
        }
    }

    return 0;
}

void Limit(uint8_t *value){
	*value=(*value>200)?0:((*value>100)?100:*value);
}

void Play(){
	  __HAL_TIM_SET_AUTORELOAD(&htim3,3333);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,3333/5);
	  OLED_NewFrame();
	  OLED_PrintString(72,15, "与狗狗", &font16x16, OLED_COLOR_NORMAL);
	  OLED_PrintString(80,34, "玩耍", &font16x16, OLED_COLOR_NORMAL);
	  OLED_DrawImage(0, 0,&dog2Img,  OLED_COLOR_NORMAL);
	  OLED_ShowFrame();
	  HAL_Delay(100);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
	  HAL_Delay(600);
	  now.happy+=10;
	  Limit(&now.happy);
	  now.hungry+=5;
	  Limit(&now.hungry);
	  now.tired+=5;
	  Limit(&now.tired);
}

void Feed(){
	  __HAL_TIM_SET_AUTORELOAD(&htim3,1500);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1500/5);
	  OLED_NewFrame();
	  OLED_PrintString(72,15 , "给狗狗", &font16x16, OLED_COLOR_NORMAL);
	  OLED_PrintString(80,34 , "喂食", &font16x16, OLED_COLOR_NORMAL);
	  OLED_DrawImage(0, 0,&dog2Img,  OLED_COLOR_NORMAL);
	  OLED_ShowFrame();
	  HAL_Delay(100);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
	  HAL_Delay(600);
	  now.happy+=5;
	  Limit(&now.happy);
	  now.hungry-=10;
	  Limit(&now.hungry);
	  now.tired-=5;
	  Limit(&now.tired);
}

void Wash(){
	  __HAL_TIM_SET_AUTORELOAD(&htim3,1000);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1000/5);
	  OLED_NewFrame();
	  OLED_PrintString(72,15, "给狗狗", &font16x16, OLED_COLOR_NORMAL);
	  OLED_PrintString(80,34, "洗澡", &font16x16, OLED_COLOR_NORMAL);
	  OLED_DrawImage(0, 0,&dog2Img,  OLED_COLOR_NORMAL);
	  OLED_ShowFrame();
	  HAL_Delay(100);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
	  HAL_Delay(600);
	  now.happy+=5;
	  Limit(&now.happy);
	  now.tired-=10;
	  Limit(&now.tired);
}
