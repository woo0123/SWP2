#include <Servo.h>

// Arduino pin assignment
#define PIN_LED   9   // LED active-low
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

// Configurable parameters for sonar
#define SND_VEL 346.0     // sound velocity at 24 Celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 180.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 360.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficient to convert duration to distance

#define _EMA_ALPHA 0.3   // EMA weight of new sample (range: 0 to 1)

// Target Distance
#define _TARGET_LOW  250.0
#define _TARGET_HIGH 290.0

// Servo motor constants
#define _DUTY_MIN 500    // servo full clockwise position (0 degree)
#define _DUTY_NEU 1500   // servo neutral position (90 degree)
#define _DUTY_MAX 2500   // servo full counterclockwise position (180 degree)

// Global variables
float dist_ema, dist_prev = _DIST_MAX;  // unit: mm
unsigned long last_sampling_time;       // unit: ms
Servo myservo;

void setup() {
  // Initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);    // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);     // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn off Sonar 

  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);  // Start at neutral position (90 degrees)

  // Initialize serial port
  Serial.begin(57600);
}

void loop() {
  float dist_raw;
  
  // Wait until next sampling time. 
  if (millis() < (last_sampling_time + INTERVAL)) {
    return;
  }

  // Measure distance using ultrasonic sensor
  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  // Apply distance filter (cut off values outside range)
  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
    dist_raw = dist_prev;           // If out of range, use previous distance
    digitalWrite(PIN_LED, HIGH);    // Turn LED off
  } else if (dist_raw < _DIST_MIN) { 
    dist_raw = dist_prev;           // If too close, use previous distance
    digitalWrite(PIN_LED, HIGH);    // Turn LED off
  } else {
    dist_prev = dist_raw;           // Update previous distance with current valid value
    digitalWrite(PIN_LED, LOW);     // Turn LED on
  }

  // Apply EMA filter to smooth the distance measurement
  dist_ema = _EMA_ALPHA * dist_raw + (1 - _EMA_ALPHA) * dist_ema;

  // Adjust servo position according to filtered distance
  int servo_position = mapDistanceToServo(dist_ema);

  // Move the servo based on the mapped distance
  myservo.writeMicroseconds(servo_position);

  // Output the distance and servo position to the serial port
  Serial.print("Min:");    Serial.print(_DIST_MIN);
  Serial.print(",Low:");   Serial.print(_TARGET_LOW);
  Serial.print(",dist:");  Serial.print(dist_raw);
  Serial.print(",Servo:"); Serial.print(myservo.read());
  Serial.print(",High:");  Serial.print(_TARGET_HIGH);
  Serial.print(",Max:");   Serial.print(_DIST_MAX);
  Serial.println("");

  // Update last sampling time
  last_sampling_time = millis();
}

// Get a distance reading from USS. Return value is in millimeter.
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;  // Return distance in mm
}

// Map the distance (180mm to 360mm) to the servo position (500us to 2500us)
int mapDistanceToServo(float distance) {
  // Constrain distance to the range of 180mm to 360mm
  distance = constrain(distance, _DIST_MIN, _DIST_MAX);

  // Map distance to the servo range (500 to 2500 microseconds)
  int servo_position = map(distance, _DIST_MIN, _DIST_MAX, _DUTY_MIN, _DUTY_MAX);

  // Return the mapped servo position
  return servo_position;
}
