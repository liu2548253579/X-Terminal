/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-11-13 04:13:50
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-12-29 09:23:08
 * @FilePath: \LVGL8_ESP32_ARDUINO-master\include\hardware.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**
 * @file hardware.h
 * Include all hardware related headers
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include "TFT_eSPI.h"
#include "Adafruit_PCF8574.h"
#include "Adafruit_MPU6050.h"
#include "Adafruit_Sensor.h"
#include "BleKeyboard.h"
#include "wifi_ftp.h"
#include "lvgl.h"
#include "time.h"

#define DOWN_KEY 0
#define MEDIUM_KEY 1
#define UP_KEY 2
#define LEFT_KEY 3
#define RIGHT_KEY 4

extern uint16_t key_val;
extern bool key_value[5],change_flag;
extern int touch_val;//触控引脚
extern unsigned char lock_state;//锁屏状态0正常显示 1暗光 2灭
extern uint16_t led_bri;//LED亮度
extern uint16_t screen_bri;//屏幕背光亮度
extern uint16_t battery_vol;//当前电池电压
extern bool charge_state,usb_state,sdmmc_state,wifi_state,time_state;//充电状态,usb连接状态,SD卡连接状态，wifi连接状态

extern tm timeinfo;

extern Adafruit_PCF8574 pcf;
extern Adafruit_MPU6050 mpu;
extern sensors_event_t a, g, temp;

extern BleKeyboard bleKeyboard;

extern lv_indev_t *indev_button;//左右两个按键
extern lv_indev_t *indev_encoder;//拨码编码器



void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );
void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
void mpu6050_init (void);
void pcf8574_init (void);
void lvgl_sys_init (void);
void key_init (void);
void encoder_init (void);
void sdmmc_init (void);
void wifi_init (void);
void time_init (void);
void key_read (void *pt);
void hardware_read (void *pt);


#endif 
