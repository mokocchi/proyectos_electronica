#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include "image.h"
// ST7789 TFT module connections
#define TFT_DC    D1     // TFT DC  pin is connected to NodeMCU pin D1 (GPIO5)
#define TFT_RST   D2     // TFT RST pin is connected to NodeMCU pin D2 (GPIO4)
#define TFT_CS    D8     // TFT CS  pin is connected to NodeMCU pin D8 (GPIO15)
// initialize ST7789 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE3);    // Init ST7789 display 240x240 pixel
  pinMode(D0, OUTPUT);
  analogWrite(D0, 20);
  // if the screen is flipped, remove this command
  tft.setRotation(2);
  tft.fillScreen(ST77XX_WHITE);
}

void loop() {
  for (int i = 0; i < 1; i++) {
    tft.drawBitmap(10, 10, twitter, 100, 100, ST77XX_CYAN);
    delay(500);
    tft.drawBitmap(125, 125, twitter, 100, 100, ST77XX_CYAN);
    delay(500);
    tft.drawBitmap(10, 125, twitter, 100, 100, ST77XX_CYAN);
    delay(500);
    tft.drawBitmap(125, 10, twitter, 100, 100, ST77XX_CYAN);
    delay(500);
    tft.fillScreen(ST77XX_WHITE);
    delay(500);
  }
  for (int i = 0; i < 4; i++) {
    tft.drawBitmap(100, 100, asterisk, 30, 30, ST77XX_MAGENTA);
    delay(300);
    tft.drawBitmap(100, 100, asterisk, 30, 30, ST77XX_WHITE);
    delay(300);
  }
}
