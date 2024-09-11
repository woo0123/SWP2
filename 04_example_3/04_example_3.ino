#define PIN_LED 13
unsigned int count, toggle;

void setup()
{
  pinMode(PIN_LED,OUTPUT);
  Serial.begin(115200);
  while(!Serial)
  {
    ;
  }
  Serial.println("Hello World!");
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle);
}

void loop()
{
  Serial.println(++count);
  toggle = 1;
  digitalWrite(PIN_LED, toggle);
  delay(1000);
  toggle = 0;
  digitalWrite(PIN_LED, toggle);
  delay(1000);
}
