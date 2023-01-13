/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-11-25 17:08:08
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-12-19 07:20:11
 * @FilePath: \LVGL8_ESP32_ARDUINO-master\include\lvgl_fun.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef LVGL_FUN_H
#define LVGL_FUN_H

#include <Arduino.h>
#include "hardware.h"
#include "lv_demo.h"
#include "lvgl.h"

extern lv_obj_t *container_menu,*container_about,*container_gyro,*container_music,*container_config,*container_ftp,*container_radio,*container_lock;
extern unsigned int running_time;
void display_containers (lv_obj_t * obj);
void lvgl (void* pt);

#endif 
