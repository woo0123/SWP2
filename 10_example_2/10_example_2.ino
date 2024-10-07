#include <Servo.h>
#define PIN_SERVO 10

Servo myservo;

void setup() {
  myservo.attach(PIN_SERVO); 
  myservo.write(0);
  delay(1000);
}

void loop() {
  myservo.write(90);
  delay(225);
  myservo.write(180);
  delay(225);
  myservo.write(90);
  delay(225);
  myservo.write(0);
  delay(225);
    // add code here.
}
