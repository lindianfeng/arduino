
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <DHT.h>
#include <TimerOne.h>
#include "TM1637.h"

//max7219
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

#define CHAR_SPACING  1 // pixels between characters

//am2302
#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

TaskHandle_t taskTestTempHandle;

TaskHandle_t taskDisplayTimeHandle;

void TaskTestTemp( void *pvParameters );
void TaskDisplayTime( void *pvParameters );

void setup() {
  xTaskCreate(TaskTestTemp, // Task function
              "TaskTestTemp", // Task name
              128, // Stack size
              NULL,
              0, // Priority
              &taskTestTempHandle); // Task handler

}

void loop() {
  // put your main code here, to run repeatedly:

}

void TaskTestTemp( void *pvParameters )
{

  mx.begin();
  Wire.begin();
  dht.begin();

  static float temp_hum_val[2] = {0};
  static char message[20] = {0};
  static char humidity_temp[6] = {0};
  static char temperature_temp[6] = {0};

  for (;;) {
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    if (!dht.readTempAndHumidity(temp_hum_val)) {
      memset(humidity_temp, 0, sizeof(humidity_temp));
      memset(temperature_temp, 0, sizeof(temperature_temp));

      dtostrf(temp_hum_val[0], 4, 2, humidity_temp);
      dtostrf(temp_hum_val[1], 4, 2, temperature_temp);

      memset(message, 0, sizeof(message));

      sprintf(message, "H%s", humidity_temp);
      printText(0, MAX_DEVICES - 1, message);

      vTaskDelay( 1000 / portTICK_PERIOD_MS );


      sprintf(message, "T%s", temperature_temp);
      printText(0, MAX_DEVICES - 1, message);
    }
  }

}

void printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
// Print the text string to the LED matrix modules specified.
// Message area is padded with blank columns after printing.
{
  uint8_t   state = 0;
  uint8_t   curLen;
  uint16_t  showLen;
  uint8_t   cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do     // finite state machine to print the characters in the space available
  {
    switch (state)
    {
      case 0: // Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (*pMsg == '\0')
        {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = mx.getChar(*pMsg++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
      // !! deliberately fall through to next state to start displaying

      case 1: // display the next part of the character
        mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if ( curLen == showLen)
        {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2: // initialize state for displaying empty columns
        curLen = 0;
        state++;
      // fall through

      case 3: // display inter-character spacing or end of message padding (blank columns)
        mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;   // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}
