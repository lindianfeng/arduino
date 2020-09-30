#include <Arduino_FreeRTOS.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "DHT.h"

TaskHandle_t taskBlinkHandle;

TaskHandle_t taskDeletedHandle;

TaskHandle_t taskBlockedHandle;

TaskHandle_t taskLedMatrixHandle;

void setup() {

  /**
     Task creation
  */
  xTaskCreate(TaskBlink, // Task function
              "Blink", // Task name
              128, // Stack size
              NULL,
              0, // Priority
              &taskBlinkHandle); // Task handler

  xTaskCreate(TaskSerial,
              "Serial",
              128,
              NULL,
              2,
              NULL);

  xTaskCreate(TaskDeleted,
              "Deleted",
              64,
              NULL,
              1,
              &taskDeletedHandle);

  xTaskCreate(TaskBlocked,
              "Blocked",
              64,
              NULL,
              1,
              &taskBlockedHandle);

  xTaskCreate(TaskLedMatrix,
              "TaskLedMatrix",
              64,
              NULL,
              1,
              &taskLedMatrixHandle);

}

void loop() {}

/**
   Example of utilities usage
*/
void TaskSerial(void *pvParameters)
{
  (void) pvParameters;

  Serial.begin(9600);

  for (;;)
  {
    Serial.println("======== Tasks status ========");
    Serial.print("Tick count: ");
    Serial.print(xTaskGetTickCount());
    Serial.print(", Task count: ");
    Serial.print(uxTaskGetNumberOfTasks());

    Serial.println();
    Serial.println();

    // Serial task status
    Serial.print("- TASK ");
    Serial.print(pcTaskGetName(NULL)); // Get task name without handler https://www.freertos.org/a00021.html#pcTaskGetName
    Serial.print(", High Watermark: ");
    Serial.print(uxTaskGetStackHighWaterMark(NULL)); // https://www.freertos.org/uxTaskGetStackHighWaterMark.html


    TaskHandle_t taskSerialHandle = xTaskGetCurrentTaskHandle(); // Get current task handle. https://www.freertos.org/a00021.html#xTaskGetCurrentTaskHandle

    Serial.println();

    Serial.print("- TASK ");
    Serial.print(pcTaskGetName(taskBlinkHandle)); // Get task name with handler
    Serial.print(", High Watermark: ");
    Serial.print(uxTaskGetStackHighWaterMark(taskBlinkHandle));
    Serial.println();

    Serial.print("- TASK ");
    Serial.print(pcTaskGetName(taskDeletedHandle));
    Serial.print(", High Watermark: ");
    Serial.print(uxTaskGetStackHighWaterMark(taskDeletedHandle));
    Serial.println();

    Serial.print("- TASK ");
    Serial.print(pcTaskGetName(taskBlockedHandle));
    Serial.print(", High Watermark: ");
    Serial.print(uxTaskGetStackHighWaterMark(taskBlockedHandle));
    Serial.println();

    Serial.println();

    vTaskDelay( 5000 / portTICK_PERIOD_MS );
  }
}

/**
   Blocked tasks when run
*/
void TaskBlocked(void *pvParameters) {
  (void) pvParameters;
  for (;;)
  {
    vTaskDelay( 900000 / portTICK_PERIOD_MS );
  }
}

/**
   Deleted tasks when run
*/
void TaskDeleted(void *pvParameters) {
  (void) pvParameters;

  vTaskDelete(NULL);
}

/*
   Blink task.
   See Blink_AnalogRead example.
*/
void TaskBlink(void *pvParameters)
{
  (void) pvParameters;

  pinMode(LED_BUILTIN, OUTPUT);

  for (;;)
  {
  
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay( 250 / portTICK_PERIOD_MS );
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay( 250 / portTICK_PERIOD_MS );
  }
}

void TaskLedMatrix(void *pvParameters)
{
  (void) pvParameters;

  //max7219
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

  const uint8_t scrollSpeed = 40;    // default frame delay value
  textEffect_t scrollEffect = PA_SCROLL_LEFT;
  textPosition_t scrollAlign = PA_LEFT;
  const uint16_t scrollPause = 0; // in milliseconds

  MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

#define  BUF_SIZE  50
  char curMessage[BUF_SIZE] = { "Hello kaka!" };
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  for (;;)
  {
    if (P.displayAnimate())
    {
      P.displayReset();
    }
  }
}
