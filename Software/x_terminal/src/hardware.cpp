/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-11-13 04:18:08
 * @LastEditors: Frank Liu ljq.frank@qq.com
 * @LastEditTime: 2023-01-02 02:47:09
 * @FilePath: \LVGL8_ESP32_ARDUINO-master\src\hardware.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "hardware.h"
#include "lvgl_fun.h"
/*Change to your screen resolution*/

//按键以及mpu6050
bool key_value[5],change_flag;
unsigned char lock_state;//锁屏状态0正常显示 1暗光 2灭
uint16_t key_val=99;
int touch_val;//触控引脚
uint16_t led_bri;//LED亮度0-4095
uint16_t battery_vol;//当前电池电压
uint16_t screen_bri;//屏幕背光亮度
bool charge_state,usb_state,sdmmc_state,wifi_state,time_state;//充电状态,usb连接状态
// WiFiMulti wifiMulti;
Adafruit_PCF8574 pcf;
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

BleKeyboard bleKeyboard;


//wifi部分
const char* ssid =     "Ljq";
const char* password = "2548253579";
// const char* ssid =     "Tenda_696650";
// const char* password = "ljh12345678";
const char* username = "LJQ";
const char* userpassword = "254825";
const char* fileurl;
FtpServer ftpSrv;  

//时间
struct tm timeinfo;

//lvgl部分
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[3200];
static TFT_eSPI tft=TFT_eSPI(screenWidth, screenHeight);

lv_indev_t *indev_button;//左右两个按键
lv_indev_t *indev_encoder;//拨码编码器

void wifi_scene (void)
{
lv_obj_t * WiFi_TXT = lv_label_create(lv_scr_act());
lv_label_set_text_fmt(WiFi_TXT, "  Connecting to %s\n\n\n\nPress any key to skip",ssid);
lv_obj_center(WiFi_TXT);
lv_timer_handler();
}

void wifi_init (void)
{
static char en;
WiFi.begin(ssid, password);
Serial.printf("connecting to %s\n",ssid);
wifi_scene ();
while(en != WL_CONNECTED)
{
en=WiFi.status();
for (uint8_t p=0; p<5; p++)
{
bool trig_key[5];
trig_key[p]=pcf.digitalRead(p);
    if(trig_key[0]==0||trig_key[1]==0||trig_key[2]==0||trig_key[3]==0||trig_key[4]==0)
    {
        en=WL_CONNECTED;
        Serial.printf("skip connecting\n");
        WiFi.mode(WIFI_OFF);
    }
}

}
if(WiFi.status() == WL_CONNECTED){wifi_state=1;Serial.printf("connected to %s\n",ssid);}else{wifi_state=0;}
}

void time_init (void)
{
    if(wifi_state)
    {
        configTime(60*60*8, 0,"ntp3.aliyun.com");    // 用的阿里云的服务器
    //   configTime(60*60*8, 0,"ntp1.aliyun.com","ntp2.aliyun.com", "ntp3.aliyun.com");    // 用的阿里云的服务器
        while(!getLocalTime(&timeinfo))delay(10);
        Serial.println("Time init done");
        getLocalTime(&timeinfo);
        Serial.println(&timeinfo, "%A, %Y-%m-%d %H:%M:%S");
        time_state=1;
    }
    else
    {
        Serial.println("Time init failed"); 
        time_state=0; 
    }
    
}


void sdmmc_init (void)
{
sdmmc_state=SD_MMC.begin("/sdcard",true);
if (sdmmc_state) {Serial.println("SD card init done.");}
else{Serial.println("SD card init failed.");}
}


void mpu6050_init (void)
{
if (!mpu.begin()) {Serial.println("Failed to find MPU6050 chip");}
mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
mpu.setGyroRange(MPU6050_RANGE_250_DEG);
mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
delay(100);
}

void pcf8574_init (void)
{
if (!pcf.begin(0x20, &Wire)) {Serial.println("Failed to find PCF8574 chip");}
for (uint8_t p=0; p<8; p++) {pcf.pinMode(p, INPUT_PULLUP);}
}


//读取PCF8574的按键并传递给全局变量KEY_VAL
void key_read (void *pt)
{
while(1)
{
uint16_t trig=0,key_val_temp=99;
for (uint8_t p=0; p<5; p++) 
{
    if(key_value[p]==0)
    {
        vTaskDelay(20);
        if(key_value[p]==0)
        {
            trig=1;
            key_val_temp=p;
        }
    }
}
if(trig){trig=0;key_val=key_val_temp;}else{vTaskDelay(20);key_val=99;}
}
}

void hardware_read (void *pt)
{
static int touch_pad;//初始触摸电平
pinMode(32,INPUT);pinMode(33,INPUT);//USB状态,充电状态
screen_bri=255;
touch_pad=touchRead(T0);
while(1)
{
battery_vol=analogRead(A3);
usb_state=digitalRead(32);
charge_state=!digitalRead(33);   
touch_val=abs(touch_pad-touchRead(T0));
analogWrite(A17,screen_bri);//屏幕亮度
analogWrite(A14,led_bri);//LED亮度
mpu.getEvent(&a, &g, &temp);
if(time_state){getLocalTime(&timeinfo);}
// Serial.printf("PIT:%.2f ROL:%.2f YAW:%.2f TEM:%.2f KEY:%d TIME:%d touch:%d \r\n",a.acceleration.x,a.acceleration.y,a.acceleration.z,temp.temperature,key_val,running_time,touch_val);   
// if(wifi_state){getLocalTime(&timeinfo);}
// {Serial.println(&timeinfo, "%A, %Y-%m-%d %H:%M:%S");}
// Serial.printf("battery:%d \r\n",battery_vol);  


for (uint8_t p=0; p<5; p++) {key_value[p]=pcf.digitalRead(p);}

if(running_time>=15&&running_time<20){lock_state=1;}
if(running_time>=20){running_time=0;lock_state=2;}

if(lock_state==0){screen_bri=255;}
if(lock_state==1){screen_bri=100;}
if(lock_state==2){screen_bri=100;}


vTaskDelay(11);
}
}

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

void lvgl_sys_init (void)
{
lv_init();
tft.begin();          /* TFT init */
tft.setRotation(3); /* Landscape orientation, flipped */
lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );
/*Initialize the display*/
static lv_disp_drv_t disp_drv;
lv_disp_drv_init( &disp_drv );
/*Change the following line to your display resolution*/
disp_drv.hor_res = screenWidth;
disp_drv.ver_res = screenHeight;
disp_drv.flush_cb = my_disp_flush;
disp_drv.draw_buf = &draw_buf;
lv_disp_drv_register( &disp_drv );
}

void key_init (void)
{
//初始化按键输入
static lv_indev_drv_t indev_drv_button;
lv_indev_drv_init(&indev_drv_button);
indev_drv_button.type = LV_INDEV_TYPE_BUTTON;
indev_drv_button.read_cb = button_read;
indev_button = lv_indev_drv_register(&indev_drv_button);
static const lv_point_t btn_points[2] = {
    {160, 120},   /*LEFT_KEY*/
    {160, 120},   /*RIGHT_KEY*/
};
lv_indev_set_button_points(indev_button, btn_points); 
}

void encoder_init (void)
{
//编码器初始化
static lv_indev_drv_t indev_drv_encoder;
lv_indev_drv_init(&indev_drv_encoder);
indev_drv_encoder.type = LV_INDEV_TYPE_ENCODER;//设备类型 编码器
indev_drv_encoder.read_cb = encoder_read;//回调函数
indev_encoder = lv_indev_drv_register(&indev_drv_encoder);
}

/*读取按键是否按下，并传递键值*/
void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint8_t last_btn = 0;
    /*Get the pressed button's ID*/
    int8_t btn_act;
    btn_act = key_val;
    if(btn_act == 3 || btn_act == 4) {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act-3;
    } 
    else {
        data->state = LV_INDEV_STATE_REL;
    }
    /*Save the last pressed button's ID*/
    data->btn_id = last_btn;
}


void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static bool but_flag = true;
	lv_indev_state_t encoder_act;
    int32_t encoder_diff;
    //编码器的按键
    if(key_val==MEDIUM_KEY)	{encoder_act = LV_INDEV_STATE_PR;}	//按下
    else{encoder_act = LV_INDEV_STATE_REL;}	//松开

    //编码器左转
    if((key_val==UP_KEY)&&but_flag){encoder_diff--;but_flag = false;}
    //编码器右转
    else if((key_val==DOWN_KEY)&&but_flag){encoder_diff++;but_flag = false;}
    else {but_flag = true;}

    if (change_flag==1){change_flag=0;encoder_diff++;but_flag = false;}

    data->enc_diff = encoder_diff;
    data->state = encoder_act;
    encoder_diff = 0;
}


