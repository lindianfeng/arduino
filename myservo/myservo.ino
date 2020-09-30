#include <Servo.h>
 
#define PIN_SERVO 7
Servo myservo;
 
void setup()
{
  myservo.attach(PIN_SERVO);
}
 
void loop()
{
  myservo.write(0);
  delay(500);
  myservo.write(80);
  delay(500);
  myservo.write(160);
  delay(500);
  myservo.write(80);
  delay(500);
  myservo.write(0);
  delay(500);
}
