/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/
 /*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>     //library for OLED graphics
#include <Adafruit_SSD1306.h> //library for displaying to OLED
#include <Adafruit_BMP085.h>  //library for BMP180 
#include <MPU6050_tockn.h>    //library to read MPU6050 data

Adafruit_BMP085 bmp;          //defining the bmp180 object to be referred to as bmp
MPU6050 mpu6050(Wire);        //defining the MPU6050 object to be referred to as mpu6050

#define SCREEN_WIDTH 128      // OLED display width, in pixels 
#define SCREEN_HEIGHT 64      // OLED display height, in pixels                                                                
#define OLED_RESET     -1     // Reset pin set to -1 because sharing arduino's reset pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

void setup() {
  Serial.begin(9600);
  Wire.begin();                  // from tockn code, initializes Wire
  mpu6050.begin();               //initializes MPU6050
  mpu6050.calcGyroOffsets(true); //takes a couple seconds to find orientation of sensor

    if (!bmp.begin()) {
  Serial.println(F("Could not find a valid BMP085 sensor, check wiring!"));        // checks if BMP180 is connected, returns error message if not and creates infinite loop to halt code
  while (1) {}
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();      //The first display.display() call shows the Adafruit splash screen 
  delay(1000);            //Pause for 1 second
  display.clearDisplay(); //Clear the splash screen

  
  display.setTextSize(2);               //Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);  //Need to specify text colour even though this is a monochromatic display
  display.setCursor(10, 0);
  display.println(F("Project"));
  display.setCursor(10,30);
  display.println(F("Andromeda"));
  display.display();
  delay(1000);
  display.clearDisplay();
}

void loop() {
  
  display.setTextSize(1);                   // Draw 1X-scale text  //Reads, updates and displays altimeter data continuously 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  //display.println(F("ALT: "));
  display.print(round(bmp.readAltitude()));
  display.setCursor(10,20);
  mpu6050.update();
  display.print(round(mpu6050.getGyroAngleY()));
  display.display();
  display.clearDisplay();

  Serial.println(freeMemory());
}
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
