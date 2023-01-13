/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-11-25 17:11:21
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-12-29 09:36:10
 * @FilePath: \LVGL8_ESP32_ARDUINO-master\src\lvgl_fun.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "lvgl_fun.h"


unsigned char time_1s;
unsigned int running_time;
//容器
lv_obj_t *container_menu,*container_about,*container_gyro,*container_music,*container_config,*container_ftp,*container_radio,*container_lock;
lv_group_t *encoder_g;
//menu可控obj
// lv_obj_t *GYRO,*MUSIC,*RADIO,*FTP,*CONFIG,*ABOUT;


void display_containers (lv_obj_t * obj)
{
static lv_obj_t *last_scene=container_lock;
lv_obj_add_flag(container_ftp, LV_OBJ_FLAG_HIDDEN);
lv_obj_add_flag(container_menu, LV_OBJ_FLAG_HIDDEN);
lv_obj_add_flag(container_gyro, LV_OBJ_FLAG_HIDDEN);   
lv_obj_add_flag(container_lock, LV_OBJ_FLAG_HIDDEN);   
lv_obj_add_flag(container_music, LV_OBJ_FLAG_HIDDEN);
lv_obj_add_flag(container_radio, LV_OBJ_FLAG_HIDDEN);
lv_obj_add_flag(container_about, LV_OBJ_FLAG_HIDDEN); 
lv_obj_add_flag(container_config, LV_OBJ_FLAG_HIDDEN);

lv_obj_clear_flag(obj,LV_OBJ_FLAG_HIDDEN);
if(obj!=last_scene){change_flag=1;}
// if(lalast_scene==obj){change_flag=0;}
// lalast_scene=last_scene;
last_scene=obj;
}

static void LOCK_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {display_containers(container_menu);}
}

static void GYRO_event_handler(lv_event_t * e)//菜单界面触发陀螺仪
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {display_containers(container_gyro);}
}

static void MUSIC_event_handler(lv_event_t * e)//菜单界面触发音乐
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {display_containers(container_music);}
}

static void RADIO_event_handler(lv_event_t * e)//菜单界面触发收音机
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {display_containers(container_radio);}
}

static void FTP_event_handler(lv_event_t * e)//菜单界面触发图片
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {display_containers(container_ftp);}
}

static void CONFIG_event_handler(lv_event_t * e)//菜单界面触发设置
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {display_containers(container_config);}
}

static void ABOUT_event_handler(lv_event_t * e)//菜单界面触发关于
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){display_containers(container_about);}
}

void lock_scene (void)//锁屏界面
{
container_lock= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_lock,320,240);
lv_obj_set_style_radius(container_lock,0,0);
lv_obj_set_style_border_width(container_lock,0,0); 
lv_obj_center(container_lock);

lv_obj_t * Unlock = lv_btn_create(container_lock);
lv_obj_align(Unlock,LV_ALIGN_CENTER,0,60);
lv_obj_set_size(Unlock,50,50);
lv_obj_set_style_radius(Unlock,100,0);
lv_obj_add_event_cb(Unlock, LOCK_event_handler, LV_EVENT_CLICKED, NULL);
lv_obj_set_style_bg_color(Unlock,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(Unlock,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
lv_obj_set_style_bg_color(Unlock,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
lv_obj_t * Unlock_TXT = lv_label_create(Unlock);
lv_label_set_text_fmt(Unlock_TXT, LV_SYMBOL_RIGHT);
lv_obj_center(Unlock_TXT);
}

void lock_flush (void)//锁屏刷新
{
static bool en;
static lv_style_t font_time,font_date;
static lv_obj_t *time,*date;

if(!en){en=1;

lv_style_init(&font_time);
lv_style_set_text_font(&font_time,&lv_font_montserrat_48);
lv_style_set_text_color(&font_time,lv_color_make(0xff,0xa5,0x00));//设置字体颜色为绿色

lv_style_init(&font_date);
lv_style_set_text_color(&font_date,lv_color_make(0xff,0xa5,0x00));//设置字体颜色为绿色
// lv_obj_add_style(label,&font_style,LV_PART_ANY);
// lv_obj_add_style(label,&font_style,LV_STATE_DEFAULT);
time = lv_label_create(container_lock);
lv_label_set_text_fmt(time,"%d:%d:%d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
lv_obj_set_align(time,LV_ALIGN_TOP_MID);
lv_obj_add_style(time,&font_time,LV_PART_MAIN);

date = lv_label_create(container_lock);
lv_label_set_text_fmt(date,"%d-%d-%d,%d",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_wday);
lv_obj_set_align(date,LV_ALIGN_CENTER);
lv_obj_add_style(date,&font_date,LV_PART_MAIN);


if(time_state)
{
    static char week[6];
    if(timeinfo.tm_wday==0){strcpy(week, "Sun");}if(timeinfo.tm_wday==1){strcpy(week, "Mon");}if(timeinfo.tm_wday==2){strcpy(week, "Tues");}if(timeinfo.tm_wday==3){strcpy(week, "Wedns");}
    if(timeinfo.tm_wday==4){strcpy(week, "Thurs");}if(timeinfo.tm_wday==5){strcpy(week, "Fri");}if(timeinfo.tm_wday==6){strcpy(week, "Satur");}
    lv_label_set_text_fmt(time,"%d:%d:%d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec); 
    lv_label_set_text_fmt(date,"%d-%d-%d,%sday",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,week); 
}
else
{
    lv_label_set_text_fmt(time,"Locked"); 
    lv_label_set_text_fmt(date,"Have a nice day!"); 
}

}
if(en){

if((!lv_obj_has_flag(container_lock,LV_OBJ_FLAG_HIDDEN))&&time_1s==0)
{
    if(time_state)
    {
        static char week[6];
        if(timeinfo.tm_wday==0){strcpy(week, "Sun");}if(timeinfo.tm_wday==1){strcpy(week, "Mon");}if(timeinfo.tm_wday==2){strcpy(week, "Tues");}if(timeinfo.tm_wday==3){strcpy(week, "Wedns");}
        if(timeinfo.tm_wday==4){strcpy(week, "Thurs");}if(timeinfo.tm_wday==5){strcpy(week, "Fri");}if(timeinfo.tm_wday==6){strcpy(week, "Satur");}
        lv_label_set_text_fmt(time,"%d:%d:%d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec); 
        lv_label_set_text_fmt(date,"%d-%d-%d,%sday",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,week); 
    }
    else
    {
        lv_label_set_text_fmt(time,"Locked"); 
        lv_label_set_text_fmt(date,"Have a nice day!"); 
    }

}

}

}

void menu_scene (void)//菜单界面
{ 
container_menu= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_menu,320,240);
lv_obj_set_style_radius(container_menu,0,0);
lv_obj_set_style_border_width(container_menu,0,0); 
lv_obj_center(container_menu);

lv_obj_t * GYRO = lv_btn_create(container_menu);
lv_obj_add_event_cb(GYRO, GYRO_event_handler, LV_EVENT_CLICKED, NULL);
lv_obj_set_pos(GYRO,0,30);
lv_obj_set_size(GYRO, 70, 70);
lv_obj_set_style_bg_color(GYRO,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(GYRO,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
lv_obj_set_style_bg_color(GYRO,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
lv_obj_t * GYRO_TXT = lv_label_create(GYRO);
lv_label_set_text_fmt(GYRO_TXT, "GYRO");
lv_obj_center(GYRO_TXT);

lv_obj_t * MUSIC = lv_btn_create(container_menu);
lv_obj_add_event_cb(MUSIC, MUSIC_event_handler, LV_EVENT_CLICKED, NULL);
lv_obj_set_pos(MUSIC,100,30);
lv_obj_set_size(MUSIC, 70, 70);
lv_obj_set_style_bg_color(MUSIC,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(MUSIC,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
lv_obj_set_style_bg_color(MUSIC,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
lv_obj_t * MUSIC_TXT = lv_label_create(MUSIC);
lv_label_set_text_fmt(MUSIC_TXT, "MUSIC");
lv_obj_center(MUSIC_TXT);

lv_obj_t * RADIO = lv_btn_create(container_menu);
lv_obj_add_event_cb(RADIO, RADIO_event_handler, LV_EVENT_CLICKED, NULL);
lv_obj_set_pos(RADIO,200,30);
lv_obj_set_size(RADIO, 70, 70);
lv_obj_set_style_bg_color(RADIO,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(RADIO,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
lv_obj_set_style_bg_color(RADIO,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
lv_obj_t * RADIO_TXT = lv_label_create(RADIO);
lv_label_set_text(RADIO_TXT, "RADIO");
lv_obj_center(RADIO_TXT);

lv_obj_t * FTP = lv_btn_create(container_menu);
lv_obj_add_event_cb(FTP, FTP_event_handler, LV_EVENT_CLICKED, NULL);
lv_obj_set_pos(FTP,0,125);
lv_obj_set_size(FTP, 70, 70);
lv_obj_set_style_bg_color(FTP,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(FTP,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
lv_obj_set_style_bg_color(FTP,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
lv_obj_t * FTP_TXT = lv_label_create(FTP);
lv_label_set_text_fmt(FTP_TXT, "FTP");
lv_obj_center(FTP_TXT);

lv_obj_t * CONFIG = lv_btn_create(container_menu);
lv_obj_add_event_cb(CONFIG, CONFIG_event_handler, LV_EVENT_CLICKED, NULL);
lv_obj_set_pos(CONFIG,100,125);
lv_obj_set_size(CONFIG, 70, 70);
lv_obj_set_style_bg_color(CONFIG,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(CONFIG,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
lv_obj_set_style_bg_color(CONFIG,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
lv_obj_t * CONFIG_TXT = lv_label_create(CONFIG);
lv_label_set_text_fmt(CONFIG_TXT, "CONFIG");
lv_obj_center(CONFIG_TXT);

lv_obj_t * ABOUT = lv_btn_create(container_menu);
lv_obj_add_event_cb(ABOUT, ABOUT_event_handler, LV_EVENT_CLICKED, NULL);
lv_obj_set_pos(ABOUT,200,125);
lv_obj_set_size(ABOUT, 70, 70);
lv_obj_set_style_bg_color(ABOUT,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ABOUT,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
lv_obj_set_style_bg_color(ABOUT,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
lv_obj_t * ABOUT_TXT = lv_label_create(ABOUT);
lv_label_set_text_fmt(ABOUT_TXT, "ABOUT");
lv_obj_center(ABOUT_TXT);

// static lv_style_t FONT;
// lv_style_init(&FONT);
// lv_style_set_text_color(&FONT,lv_color_make(0xff,0x00,0x00));
// lv_obj_add_style(GYRO_TXT,&FONT,LV_PART_MAIN);
// lv_obj_add_style(MUSIC_TXT,&FONT,LV_PART_MAIN);
// lv_obj_add_style(RADIO_TXT,&FONT,LV_PART_MAIN);
// lv_obj_add_style(FTP_TXT,&FONT,LV_PART_MAIN);
// lv_obj_add_style(CONFIG_TXT,&FONT,LV_PART_MAIN);
// lv_obj_add_style(ABOUT_TXT,&FONT,LV_PART_MAIN);


lv_obj_t * sdcard_icon = lv_label_create(container_menu);
if(sdmmc_state){lv_label_set_text(sdcard_icon, LV_SYMBOL_SD_CARD);}
else {lv_label_set_text(sdcard_icon, LV_SYMBOL_CLOSE);}
lv_obj_set_pos(sdcard_icon,205,-20);
lv_obj_set_size(sdcard_icon,15,15);

lv_obj_t * WiFi_icon = lv_label_create(container_menu);
if(wifi_state)lv_label_set_text(WiFi_icon, LV_SYMBOL_WIFI);
else lv_label_set_text(WiFi_icon, LV_SYMBOL_CLOSE);
lv_obj_set_pos(WiFi_icon,220,-20);
lv_obj_set_size(WiFi_icon,15,15);
}

void menu_flush (void)//菜单刷新
{
static lv_obj_t *charge_icon,*usb_icon,*timeinfo_icon;
static bool en;
if(!en){en=1;
charge_icon = lv_label_create(container_menu);
lv_obj_set_pos(charge_icon,240,-20);
lv_obj_set_size(charge_icon,15,15);
if(charge_state){lv_label_set_text(charge_icon, LV_SYMBOL_BATTERY_FULL);}
if(usb_state&&!charge_state) {lv_label_set_text(charge_icon, LV_SYMBOL_CHARGE);}
if(!usb_state&&!charge_state) {lv_label_set_text(charge_icon, LV_SYMBOL_BATTERY_3);}

usb_icon = lv_label_create(container_menu);
lv_obj_set_pos(usb_icon,260,-20);
lv_obj_set_size(usb_icon,15,15);
if(usb_state)lv_label_set_text(usb_icon, LV_SYMBOL_USB);
else lv_label_set_text(usb_icon, LV_SYMBOL_CLOSE);

timeinfo_icon = lv_label_create(container_menu);
lv_obj_set_pos(timeinfo_icon,0,-20);
if(time_state){lv_label_set_text_fmt(timeinfo_icon,"%d-%d-%d  %d:%d:%d",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);}
else{lv_label_set_text_fmt(timeinfo_icon,"Cherish your every second");}
// lv_obj_set_size(usb_icon,15,15);
// lv_obj_set_align(timeinfo_icon,LV_ALIGN_TOP_LEFT);

}
if(en){
    if((!lv_obj_has_flag(container_menu,LV_OBJ_FLAG_HIDDEN))&&time_1s==0)
    {
        if(charge_state){lv_label_set_text(charge_icon, LV_SYMBOL_BATTERY_FULL);}
        if(usb_state&&!charge_state) {lv_label_set_text(charge_icon, LV_SYMBOL_CHARGE);}
        if(!usb_state&&!charge_state) {lv_label_set_text(charge_icon, LV_SYMBOL_BATTERY_3);}

        if(usb_state)lv_label_set_text(usb_icon, LV_SYMBOL_USB);
        else lv_label_set_text(usb_icon, LV_SYMBOL_CLOSE);

        if(time_state){lv_label_set_text_fmt(timeinfo_icon,"%d-%d-%d  %d:%d:%d",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);}
        else{lv_label_set_text_fmt(timeinfo_icon,"Cherish your every second");}
    }
}

}

void gyro_scene (void)//陀螺仪界面
{ 
container_gyro= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_gyro,320,240);
lv_obj_set_style_radius(container_gyro,0,0);
lv_obj_set_style_border_width(container_gyro,0,0); 
lv_obj_center(container_gyro);

// lv_obj_t * back = lv_btn_create(container_gyro);
// lv_obj_align(back,LV_ALIGN_CENTER,0,80);
// lv_obj_set_size(back,50,20);
// lv_obj_set_style_radius(back,5,0);
// lv_obj_add_event_cb(back, LOCK_event_handler, LV_EVENT_CLICKED, NULL);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
// lv_obj_set_style_bg_color(back,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
// lv_obj_t * back_TXT = lv_label_create(back);
// lv_label_set_text_fmt(back_TXT, "Back");
// lv_obj_center(back_TXT);
}

void gyro_flush (void)//陀螺仪刷新
{
static lv_obj_t *point_gyro,*point_gyro_text,*accx,*accx_text,*accy,*accy_text,*accz,*accz_text,*gyrox,*gyrox_text,*gyroy,*gyroy_text,*gyroz,*gyroz_text,*temp_arc,*temp_arc_text;
static bool en;
if(!en){en=1;
point_gyro = lv_led_create(container_gyro);
lv_obj_set_size(point_gyro, 8, 8);
lv_obj_center(point_gyro);
point_gyro_text=lv_label_create(point_gyro);
lv_label_set_text(point_gyro_text,"A");
lv_obj_center(point_gyro_text);

accx = lv_bar_create(container_gyro);
lv_obj_set_size(accx, 100, 10);
lv_bar_set_range(accx,-10,10);
lv_obj_set_pos(accx,20,-17);
accx_text=lv_label_create(container_gyro);
lv_obj_set_pos(accx_text,-20,-20);

accy = lv_bar_create(container_gyro);
lv_obj_set_size(accy, 100, 10);
lv_bar_set_range(accy,-10,10);
lv_obj_set_pos(accy,20,3);
accy_text=lv_label_create(container_gyro);
lv_obj_set_pos(accy_text,-20,0);

accz = lv_bar_create(container_gyro);
lv_obj_set_size(accz, 100, 10);
lv_bar_set_range(accz,-10,10);
lv_obj_set_pos(accz,20,23);
accz_text=lv_label_create(container_gyro);
lv_obj_set_pos(accz_text,-20,20);

gyrox = lv_bar_create(container_gyro);
lv_obj_set_size(gyrox, 100, 10);
lv_bar_set_range(gyrox,-10,10);
lv_obj_set_pos(gyrox,165,-17);
gyrox_text=lv_label_create(container_gyro);
lv_obj_set_pos(gyrox_text,125,-20);

gyroy = lv_bar_create(container_gyro);
lv_obj_set_size(gyroy, 100, 10);
lv_bar_set_range(gyroy,-10,10);
lv_obj_set_pos(gyroy,165,3);
gyroy_text=lv_label_create(container_gyro);
lv_obj_set_pos(gyroy_text,125,0);

gyroz = lv_bar_create(container_gyro);
lv_obj_set_size(gyroz, 100, 10);
lv_bar_set_range(gyroz,-10,10);
lv_obj_set_pos(gyroz,165,23);
gyroz_text=lv_label_create(container_gyro);
lv_obj_set_pos(gyroz_text,125,20);

temp_arc = lv_arc_create(container_gyro);
lv_obj_set_size(temp_arc, 110, 110);
lv_arc_set_range(temp_arc,0,100);
lv_arc_set_rotation(temp_arc, 135);
lv_arc_set_bg_angles(temp_arc, 0, 270);
lv_obj_set_align(temp_arc,LV_ALIGN_CENTER);
lv_obj_set_pos(temp_arc,80,20);
temp_arc_text = lv_label_create(temp_arc);
lv_obj_center(temp_arc_text);

static lv_style_t temp_arc_style;
lv_style_init(&temp_arc_style);
lv_style_set_arc_width(&temp_arc_style,5);
lv_obj_add_style(temp_arc,&temp_arc_style,LV_PART_MAIN);
lv_obj_add_style(temp_arc,&temp_arc_style,LV_PART_INDICATOR);
lv_obj_add_style(temp_arc,&temp_arc_style,LV_PART_ITEMS);
lv_obj_add_style(temp_arc,&temp_arc_style,LV_PART_SCROLLBAR);
}

if(en){
    if(!lv_obj_has_flag(container_gyro,LV_OBJ_FLAG_HIDDEN))
    {
        lv_bar_set_value(accx,int(a.acceleration.x), LV_ANIM_OFF);
        lv_label_set_text_fmt(accx_text,"ax:%d",int(a.acceleration.x));
        lv_bar_set_value(accy,int(a.acceleration.y), LV_ANIM_OFF);
        lv_label_set_text_fmt(accy_text,"ay:%d",int(a.acceleration.y));
        lv_bar_set_value(accz,int(a.acceleration.z), LV_ANIM_OFF);
        lv_label_set_text_fmt(accz_text,"az:%d",int(a.acceleration.z));

        lv_bar_set_value(gyrox,int(g.gyro.x), LV_ANIM_OFF);
        lv_label_set_text_fmt(gyrox_text,"gx:%d",int(g.gyro.x));
        lv_bar_set_value(gyroy,int(g.gyro.y), LV_ANIM_OFF);
        lv_label_set_text_fmt(gyroy_text,"gy:%d",int(g.gyro.y));
        lv_bar_set_value(gyroz,int(g.gyro.z), LV_ANIM_OFF);
        lv_label_set_text_fmt(gyroz_text,"gz:%d",int(g.gyro.z));

        lv_arc_set_value(temp_arc, int(temp.temperature));
        lv_label_set_text_fmt(temp_arc_text,"temp:%.1f",temp.temperature);
        
        lv_obj_set_pos(point_gyro,-80+int(-a.acceleration.y*5),20+int(-a.acceleration.x*5));
        Serial.printf("PIT:%.2f ROL:%.2f YAW:%.2f TEM:%.2f \r\n",a.acceleration.x,a.acceleration.y,a.acceleration.z,temp.temperature);   
    }
}


}

void music_scene (void)//音乐界面
{
container_music= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_music,320,240);
lv_obj_set_style_radius(container_music,0,0);
lv_obj_set_style_border_width(container_music,0,0);
lv_obj_center(container_music);  
 
// lv_obj_t * back = lv_btn_create(container_music);
// lv_obj_align(back,LV_ALIGN_CENTER,0,80);
// lv_obj_set_size(back,50,20);
// lv_obj_set_style_radius(back,5,0);
// lv_obj_add_event_cb(back, LOCK_event_handler, LV_EVENT_CLICKED, NULL);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
// lv_obj_set_style_bg_color(back,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
// lv_obj_t * back_TXT = lv_label_create(back);
// lv_label_set_text_fmt(back_TXT, "Back");
// lv_obj_center(back_TXT);
}

void radio_scene (void)//收音机界面
{
container_radio= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_radio,320,240);
lv_obj_set_style_radius(container_radio,0,0);
lv_obj_set_style_border_width(container_radio,0,0);   
lv_obj_center(container_radio);  

// lv_obj_t * back = lv_btn_create(container_radio);
// lv_obj_align(back,LV_ALIGN_CENTER,0,80);
// lv_obj_set_size(back,50,20);
// lv_obj_set_style_radius(back,5,0);
// lv_obj_add_event_cb(back, LOCK_event_handler, LV_EVENT_CLICKED, NULL);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
// lv_obj_set_style_bg_color(back,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
// lv_obj_t * back_TXT = lv_label_create(back);
// lv_label_set_text_fmt(back_TXT, "Back");
// lv_obj_center(back_TXT);
}

void ftp_scene (void)//FTP界面
{
container_ftp= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_ftp,320,240);
lv_obj_set_style_radius(container_ftp,0,0);
lv_obj_set_style_border_width(container_ftp,0,0);  
lv_obj_center(container_ftp);  

// lv_obj_t * back = lv_btn_create(container_ftp);
// lv_obj_align(back,LV_ALIGN_CENTER,0,80);
// lv_obj_set_size(back,50,20);
// lv_obj_set_style_radius(back,5,0);
// lv_obj_add_event_cb(back, LOCK_event_handler, LV_EVENT_CLICKED, NULL);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
// lv_obj_set_style_bg_color(back,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
// lv_obj_t * back_TXT = lv_label_create(back);
// lv_label_set_text_fmt(back_TXT, "Back");
// lv_obj_center(back_TXT);
}

void config_scene (void)//设置界面
{ 
container_config= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_config,320,240);
lv_obj_set_style_radius(container_config,0,0); 
lv_obj_set_style_border_width(container_config,0,0);   
lv_obj_center(container_config); 

// lv_obj_t * back = lv_btn_create(container_config);
// lv_obj_align(back,LV_ALIGN_CENTER,0,80);
// lv_obj_set_size(back,50,20);
// lv_obj_set_style_radius(back,5,0);
// lv_obj_add_event_cb(back, LOCK_event_handler, LV_EVENT_CLICKED, NULL);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
// lv_obj_set_style_bg_color(back,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
// lv_obj_t * back_TXT = lv_label_create(back);
// lv_label_set_text_fmt(back_TXT, "Back");
// lv_obj_center(back_TXT);
}

void about_scene (void)//关于界面
{
container_about= lv_obj_create(lv_scr_act());
lv_obj_set_size(container_about,320,240);
lv_obj_set_style_radius(container_about,0,0);
lv_obj_set_style_border_width(container_about,0,0);     
lv_obj_center(container_about);  

lv_obj_t * about = lv_label_create(container_about);
lv_obj_set_align(about,LV_ALIGN_CENTER);
lv_obj_center(about);
lv_label_set_text_fmt(about, "about");

// lv_obj_t * back = lv_btn_create(container_about);
// lv_obj_align(back,LV_ALIGN_CENTER,0,80);
// lv_obj_set_size(back,50,20);
// lv_obj_set_style_radius(back,5,0);
// lv_obj_add_event_cb(back, LOCK_event_handler, LV_EVENT_CLICKED, NULL);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0xbf,0xff),LV_STATE_DEFAULT);
// lv_obj_set_style_bg_color(back,lv_color_make(0x00,0x90,0xff),LV_STATE_FOCUSED);
// lv_obj_set_style_bg_color(back,lv_color_make(0x1e,0x60,0xff),LV_STATE_PRESSED);
// lv_obj_t * back_TXT = lv_label_create(back);
// lv_label_set_text_fmt(back_TXT, "Back");
// lv_obj_center(back_TXT);
}



void time_lock (void)//锁屏时间与系统时钟
{
    if(key_val==99&&lock_state!=2){time_1s++;}//按键未触发且未锁屏 则计时
    else{time_1s=0;running_time=0;}//按键触发或者锁屏停止计时

    if(key_val!=99&&lock_state==1){lock_state=0;}//按键触发且已经暗屏 则亮屏重置状态
    if(key_val==99&&lock_state==2){display_containers(container_lock);}//按键未触发且已经锁屏 则切换为锁屏状态
    if(key_val!=99&&lock_state==2){lock_state=0;}//按键触发且已经锁屏 则亮屏重置状态
    if(time_1s==200){time_1s=0;running_time+=1;}
}

// void lvgl_flush (void)
// {
//     while(1)
//     {


//    gyro_flush ();

//     if(key_val==1){led_bri=4095;}else{led_bri=0;}

//     if(key_val==3&&lv_obj_has_flag(container_menu,LV_OBJ_FLAG_HIDDEN)&&lv_obj_has_flag(container_lock,LV_OBJ_FLAG_HIDDEN))//返回主界面
//     {
//         display_containers(container_menu);
//     }

//     // if(!lv_obj_has_flag(container_lock,LV_OBJ_FLAG_HIDDEN))
//     // {
//     //     // if(key_val==4){display_containers(container_menu);}
//     // }
    
//     if(lv_obj_has_flag(container_lock,LV_OBJ_FLAG_HIDDEN))
//     {
//        if(key_val==4){display_containers(container_lock);} 
//     }

//     time_lock ();
//     lv_timer_handler(); /* let the GUI do its work */
//     vTaskDelay(5);   
//     } 
// }



void init_scene (void)
{
lv_group_t *encoder_g= lv_group_create();
lv_indev_set_group(indev_encoder,encoder_g);
lv_group_set_default(encoder_g);         //设置组(group)为默认模式,后面的部件创建时会自动加入组(group)

lock_scene ();
menu_scene ();
gyro_scene ();
ftp_scene ();
config_scene();
music_scene ();
radio_scene ();
about_scene();

display_containers(container_lock);
}

void lvgl (void *pt)
{
init_scene();
/*LOOP*/
    while(1)
    {

        menu_flush();
        gyro_flush ();
        lock_flush ();
        if(key_val==1){led_bri=4095;}else{led_bri=0;}

        if(key_val==3&&lv_obj_has_flag(container_menu,LV_OBJ_FLAG_HIDDEN)&&lv_obj_has_flag(container_lock,LV_OBJ_FLAG_HIDDEN))//返回主界面
        {
            display_containers(container_menu);
        }

        // if(!lv_obj_has_flag(container_lock,LV_OBJ_FLAG_HIDDEN))
        // {
        //     // if(key_val==4){display_containers(container_menu);}
        // }
        
        if(lv_obj_has_flag(container_lock,LV_OBJ_FLAG_HIDDEN))
        {
            if(key_val==4){key_val=99;lock_state=2;} 
        }




        time_lock ();
        lv_timer_handler(); /* let the GUI do its work */
        vTaskDelay(5);   
    } 

}

