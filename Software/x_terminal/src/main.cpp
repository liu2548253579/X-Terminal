/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-09-25 06:04:40
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-12-29 09:36:20
 * @FilePath: \LVGL8_ESP32_ARDUINO-master\src\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "hardware.h"
#include "lvgl_fun.h"

// void change (void* pt)
// {
// while (1)
// {

//             if(bleKeyboard.isConnected()) 
//         {
//             if(key_val==0)bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
//             if(key_val==1)bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
//             if(key_val==2)bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
//             if(key_val==3)bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
//             if(key_val==4)bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);

//         }
//         vTaskDelay(200);
//     /* code */
// }


// }


void setup()
{
Serial.begin(115200);//初始化串口

// bleKeyboard.begin();

// bleKeyboard.end();

mpu6050_init();//初始化mpu6050

pcf8574_init();//初始化pcf8574

lvgl_sys_init();//初始化lvgl
// key_init();//初始化lvgl按键
encoder_init();//初始化lvgl编码器

sdmmc_init();//初始化内存卡

wifi_init();//初始化初始化WiFi

time_init ();

// WiFi.mode(WIFI_OFF);

// bleKeyboard.begin();

xTaskCreate(hardware_read,"hardware_read",4096,NULL,4,NULL);//基础硬件读取
xTaskCreate(lvgl,"lvgl",4096,NULL,2,NULL);//创建lvgl任务
xTaskCreate(key_read,"key_read",2048,NULL,5,NULL);//读取按键键值
// xTaskCreate(change,"key_read",2048,NULL,5,NULL);//读取按键键值
Serial.println( "Setup done" );//提示初始化完毕
// vTaskStartScheduler();//开启freertos任务调度
}

void loop()
{

}
