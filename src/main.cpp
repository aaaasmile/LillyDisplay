#include <Adafruit_I2CDevice.h>
#include <Arduino.h>

#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "GxEPD2_display_selection.h"
#include "GxEPD2_display_selection_added.h"
#include <qrcode.h> 
#include "inidata.inc"

char g_Name[25] = "";
char g_Descr[25] = "";
char g_Line1[25] = DATA_LN_1;
char g_Line2[25] = DATA_LN_2;
char g_Line3[25] = DATA_LN_3;
char g_Line4[25] = DATA_LN_4;
bool g_textHasChanged = false;
QRCode g_qrcode;


void printNameDescr() {
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

// Visit card is is 4 rows contact information + qr code for whatsup chatme
// Tel number is on line 4
void print_visit_card(){
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();
  int x = 103;
  int y = 31;
  int lineSTep = 20;
  int offset_x = 0;
  int offset_y = 10; 
  int element_size = 3;
  int QRsize = 4;
  uint8_t qrcodeData[qrcode_getBufferSize(QRsize)];
  String message;
  message =  "https://wa.me/" + String(g_Line4) + "?text=Hallo!";
  qrcode_initText(&g_qrcode, qrcodeData, QRsize, ECC_LOW, message.c_str());

  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    
    for (int y = 0; y < g_qrcode.size; y++) {
      for (int x = 0; x < g_qrcode.size; x++) {
        if (qrcode_getModule(&g_qrcode, x, y)) {
            display.fillRect(x*element_size+offset_x,y*element_size+offset_y,element_size,element_size,GxEPD_BLACK);
        }
        else 
        {
            display.fillRect(x*element_size+offset_x,y*element_size+offset_y,element_size,element_size,GxEPD_WHITE);
        }
      }
    }

    display.setCursor(x, y);
    display.print(g_Line1);
    y += lineSTep;
    display.setCursor(x, y);
    display.print(g_Line2);
    y += lineSTep;
    display.setCursor(x, y);
    display.print(g_Line3);
    y += lineSTep;
    display.setCursor(x, y);
    display.print(g_Line4);
    
  }while (display.nextPage());  
}

void Display_QRcode(int offset_x, int offset_y, int element_size, int QRsize, int ECC_Mode, const char* Message){
  uint8_t qrcodeData[qrcode_getBufferSize(QRsize)];
  display.setRotation(1);
  display.setTextColor(GxEPD_BLACK);

  //ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH. Higher levels of error correction sacrifice data capacity, but ensure damaged codes remain readable.
  if (ECC_Mode%4 == 0) qrcode_initText(&g_qrcode, qrcodeData, QRsize, ECC_LOW, Message);
  if (ECC_Mode%4 == 1) qrcode_initText(&g_qrcode, qrcodeData, QRsize, ECC_MEDIUM, Message);
  if (ECC_Mode%4 == 2) qrcode_initText(&g_qrcode, qrcodeData, QRsize, ECC_QUARTILE, Message);
  if (ECC_Mode%4 == 3) qrcode_initText(&g_qrcode, qrcodeData, QRsize, ECC_HIGH, Message);

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    for (int y = 0; y < g_qrcode.size; y++) {
      for (int x = 0; x < g_qrcode.size; x++) {
        if (qrcode_getModule(&g_qrcode, x, y)) {
            //display.fillRect(x, y, w, h, GxEPD_BLACK); // refernce
            display.fillRect(x*element_size+offset_x,y*element_size+offset_y,element_size,element_size,GxEPD_BLACK);
        }
        else 
        {
            display.fillRect(x*element_size+offset_x,y*element_size+offset_y,element_size,element_size,GxEPD_WHITE);
        }
      }
    }
    display.setRotation(1);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(120, 31);
    display.print(g_Line1);
    display.setCursor(120, 51);
    display.print(g_Line2);
    display.setCursor(120, 71);
    display.print(g_Line3);

   } while (display.nextPage());  
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
    print_visit_card();
    g_textHasChanged = false;
  }
  delay(100);
}