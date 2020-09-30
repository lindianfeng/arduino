#include <MD_MAX72xx.h>
#include <SPI.h>
#include <DHT.h>
#include <TimerOne.h>
#include "TM1637.h"

#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }

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

//tim1637

int8_t TimeDisp[] = {0x00, 0x00, 0x00, 0x00};
unsigned char ClockPoint = 1;
unsigned char Update = 0;
unsigned char halfsecond = 0;
unsigned char second;
unsigned char minute = 0;
unsigned char hour = 0;

#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3
TM1637 tm1637(CLK, DIO);

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

void setup()
{
  mx.begin();

  Serial.begin(9600 );
  Serial.print("\n[MD_MAX72XX Message Display]\nType a message for the display\nEnd message line with a newline");
  
  //am2302
  Wire.begin();
  dht.begin();
  
  //
  tm1637.set();
  tm1637.init();
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR
}

static float temp_hum_val[2] = {0};
static char message[20] = {0};
static char humidity_temp[6] = {0};
static char temperature_temp[6] = {0};

static unsigned int counter = 0;
void loop()
{
  if ((counter % 100 == 0 || counter % 300 == 0) && !dht.readTempAndHumidity(temp_hum_val)) {
    memset(humidity_temp, 0, sizeof(humidity_temp));
    memset(temperature_temp, 0, sizeof(temperature_temp));

    dtostrf(temp_hum_val[0], 4, 2, humidity_temp);
    dtostrf(temp_hum_val[1], 4, 2, temperature_temp);

    memset(message, 0, sizeof(message));

    if (counter % 100 == 0)
    {
      sprintf(message, "H%s", humidity_temp);
      printText(0, MAX_DEVICES - 1, message);
    }

    if (counter % 300 == 0)
    {
      sprintf(message, "T%s", temperature_temp);
      printText(0, MAX_DEVICES - 1, message);
    }
  }

  if (Update == 1) {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }

  ++counter;
}

void TimingISR() {
  halfsecond ++;
  Update = 1;
  if (halfsecond == 2) {
    second ++;
    if (second == 60) {
      minute ++;
      if (minute == 60) {
        hour ++;
        if (hour == 24) {
          hour = 0;
        }
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;
  }
  // Serial.println(second);
  ClockPoint = (~ClockPoint) & 0x01;
}

void TimeUpdate(void) {
  if (ClockPoint) {
    tm1637.point(POINT_ON);
  } else {
    tm1637.point(POINT_OFF);
  }
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
  Update = 0;
}
