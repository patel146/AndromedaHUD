#include "U8glib.h"                                              //Include the graphics library.
#include <Adafruit_BMP085.h>                                     //library for BMP180
#include <Wire.h>
#include <MPU6050_tockn.h>                                       //library to read MPU6050 data
#include <AltSoftSerial.h>
#include "TinyGPS++.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); //Initialize display.
Adafruit_BMP085 bmp;                                             //defining the bmp180 object to be referred to as bmp
MPU6050 mpu6050(Wire);                                           //defining the MPU6050 object to be referred to as mpu6050
AltSoftSerial as;
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data

int alt;
int bank;
int gs;

void setup(void)
{
  Serial.begin(9600);
  while(!Serial);
  as.begin(9600);
  Wire.begin();
  mpu6050.begin();               //initializes MPU6050
  mpu6050.calcGyroOffsets(true); //takes a couple seconds to find orientation of sensor
    
    u8g.setFont(u8g_font_unifont);  //Set font.

        if (!bmp.begin()) {
    Serial.println(F("No BMP085"));        // checks if BMP180 is connected, returns error message if not and creates infinite loop to halt code
    while (1) {}
  }
}

void loop(void)
{
  alt = bmp.readAltitude();
  bank = mpu6050.getGyroAngleY();

  mpu6050.update();

  while(as.available())
  {
    gps.encode(as.read());
  }
  if(gps.location.isUpdated())
  {
    gs = gps.speed.mph();
  }
  
    u8g.firstPage();
    do {
        draw();
    } while (u8g.nextPage());
}
void draw(void)
{
    u8g.setPrintPos(98, 28);
    u8g.print(alt);
    u8g.setPrintPos(64, 55);
    u8g.print(bank);
    u8g.setPrintPos(10, 28);
    u8g.print(gs);
    
}
