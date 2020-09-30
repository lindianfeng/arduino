#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include "DHT.h"

//serial
#define debug  Serial

//lcd
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

//am2302
#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

//max7219
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const uint8_t scrollSpeed = 40;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
const uint16_t scrollPause = 500; // in milliseconds

#define  BUF_SIZE  50
char curMessage[BUF_SIZE] = { "Hello kaka!" };

//
volatile int REV = 0;
const int interruptPin = 2;

//led
#define LED_PIN    8
volatile int led = 0;
volatile byte led_status = 1;


void setup()
{
  //serial
  debug.begin(9600);
  debug.print("\nhello kaka!\n");

  //lcd
  lcd.begin(16, 2);      //设置LCD的行列数
  lcd.print("hello, world!");

  //am2302
  Wire.begin();
  dht.begin();

  //matrix
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

  //
  //attachInterrupt(digitalPinToInterrupt(interruptPin), rpmCount, RISING);

  //buildin led
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (P.displayAnimate())
  {
    float temp_hum_val[2] = {0};
    if (!dht.readTempAndHumidity(temp_hum_val)) {
      char humidity_temp[6];
      char temperature_temp[6];
      dtostrf(temp_hum_val[0], 4, 2, humidity_temp);
      dtostrf(temp_hum_val[1], 4, 2, temperature_temp);
      sprintf(curMessage, "H:%s%%,T:%s*C", humidity_temp, temperature_temp);
      debug.println(curMessage);
      lcd.print(curMessage);

      if (led_status) {
        digitalWrite(LED_BUILTIN, HIGH );
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }

      led_status = !led_status;

    } else {
      debug.println("Failed to get temprature and humidity value.");
    }

    P.displayReset();
  }
}

void rpmCount()
{
  REV++;
  debug.print("rev:");
  debug.println(REV);
  if (led == LOW)
  {
    led = HIGH;
  }
  else
  {
    led = LOW;
  }

  digitalWrite(LED_PIN, led);
}
