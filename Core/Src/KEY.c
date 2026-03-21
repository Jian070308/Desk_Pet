#include "KEY.h"

#define KEY_DEBOUNCE_TIME 20

extern TIM_HandleTypeDef htim3;

extern char message[20];

// 定义一个全局“信箱”，用于在后台中断和主循环之间传递按键值
volatile uint8_t global_key_value = 0;

/* * 后台按键扫描函数 (将在 1ms 中断中自动调用)
 * 它使用状态机逻辑，完美避开所有阻塞问题
 */
void Key_Interrupt_Scan(void) {
    static uint8_t key_state = 0;
    static uint16_t delay_count = 0;
    static uint8_t current_key = 0;

    // 1. 读取当前物理按键状态
    uint8_t key_read = 0;
    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) key_read = 1;
    else if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET) key_read = 2;
    else if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET) key_read = 3;
    else if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET) key_read = 4;

    // 2. 状态机处理
    switch (key_state) {
        case 0: // 状态0：空闲，等待按下
            if (key_read != 0) {
                current_key = key_read; // 记录是哪个键按下的
                key_state = 1;          // 进入消抖状态
                delay_count = 0;
            }
            break;

        case 1: // 状态1：消抖确认
            if (key_read == current_key) {
                delay_count++;
                if (delay_count >= 20) { // 连续 20 毫秒确认按下，消抖成功！
                    global_key_value = current_key; // 将按键值放入全局“信箱”
                    key_state = 2;                  // 进入等待松手状态
                }
            } else {
                key_state = 0; // 如果这 20ms 内状态变了，说明是杂波/抖动，复位
            }
            break;

        case 2: // 状态2：等待松开 (防止长按导致疯狂触发)
            if (key_read == 0) {
                key_state = 0; // 彻底松手了，回到状态0准备下一次按键
            }
            break;
    }
}

/*
 * 主循环调用的获取按键函数
 * 只需要看一眼“信箱”里有没有中断扫到的按键即可，极速返回！
 */
int KEY_Scan(void){
    uint8_t temp = global_key_value;
    global_key_value = 0; // 读取完毕后清空信箱
    return temp;
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
