#include <Adafruit_I2CDevice.h>
#include <Arduino.h>

#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "GxEPD2_display_selection.h"
#include "GxEPD2_display_selection_added.h"

char g_Name[25] = "";
char g_Descr[25] = "";
bool g_textHasChanged = false;

void printName() {
  Serial.println("printName - begin");

  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);

  //Serial.print("display.epd2.WIDTH: ");Serial.print(display.epd2.WIDTH);Serial.print(", height: ");Serial.print(display.height());Serial.print(", width: ");Serial.println(display.width());

  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(g_Name, 0, 0, &tbx, &tby, &tbw, &tbh);
  //Serial.print("Text bounds. tbx : ");Serial.print(tbx);Serial.print(", tby: ");Serial.print(tby);Serial.print(", tbw: ");Serial.print(tbw);Serial.print(", tbh: ");Serial.println(tbh);

  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  //uint16_t y = ((display.height() - tbh) / 2) - tby;
  uint16_t y = tbh + 1;
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(g_Name);
    y = y + tbh + 1;
    display.setCursor(x, y);
    display.print(g_Descr);
  } while (display.nextPage());

  Serial.println("printName - end");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  delay(100);
  display.init(115200);
  delay(1000);
  g_textHasChanged = true;
}

void loop() {
  if (g_textHasChanged) {
    strncpy(g_Name, "Igor Sarzi Sartori", 25);
    strncpy(g_Descr, "QR Green Pass", 25);
    printName();
    g_textHasChanged = false;
  }
  delay(100);
}