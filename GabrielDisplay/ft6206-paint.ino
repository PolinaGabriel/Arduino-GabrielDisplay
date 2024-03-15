#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

Adafruit_FT6206 ctp = Adafruit_FT6206();
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup(void) {
  Serial.begin(115200);
  tft.begin();
  if (! ctp.begin(40)) {
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }
  tft.fillScreen(ILI9341_BLACK); // чёрный экран
  tft.fillRect(20, 20, 80, 80, ILI9341_RED); // кнопка выкл
  tft.fillRect(140, 20, 80, 80, ILI9341_GREEN); // кнопка вкл
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(35, 50); // надпись на кнопке выкл
  tft.print("OFF");
  tft.setCursor(165, 50); // надпись на кнопке вкл
  tft.print("ON");
  tft.setCursor(30, 250); // координата x курсора
  tft.print("X = ");
  tft.setCursor(30, 280); // координата y курсора
  tft.print("Y = ");
  pinMode(2, OUTPUT);
}

void loop() {
  if (! ctp.touched()) {
    return;
  }
  TS_Point p = ctp.getPoint();
  p.x = map(p.x, 0, 240, 240, 0);
  p.y = map(p.y, 0, 320, 320, 0);
  Serial.print("("); Serial.print(p.x); // вывод координат курсора
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
  tft.fillRect(100, 250, 52, 51, ILI9341_BLACK); // очистка экрана
  tft.fillRect(33, 150, 180, 21, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(100, 250); // координата x курсора
  tft.print(p.x);
  tft.setCursor(100, 280); // координата y курсора
  tft.print(p.y);
  tft.setTextColor(ILI9341_BLUE);
  if (p.y < 100) { // включение/выключение лампочки по кнопкам
     if (p.x < 130) { 
       digitalWrite(2, LOW);
       tft.setCursor(33, 150); // сообщение о выключении
       tft.print("led is off");
     } else if (p.x < 240) {
       digitalWrite(2, HIGH);
       tft.setCursor(40, 150); // сообщение о включении
       tft.print("led is on");
     }
  }
}