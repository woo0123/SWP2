#define PIN_LED 13
unsigned int count, toggle = 0; // 전역 변수로 초기화

void setup() {
  pinMode(PIN_LED, OUTPUT);  
  Serial.begin(115200);      
  while (!Serial) {
    ;  
  }
}

void loop() {
  // LED를 1초 동안 켭니다.
  toggle = 1;
  digitalWrite(PIN_LED, toggle);  
  Serial.println("1초 불"); // 현재 count 값을 출력합니다.
  delay(1000);  

  // count가 5보다 작을 때까지 반복
  while (count < 5) {
    toggle = 0;
    digitalWrite(PIN_LED, toggle);  // LED 끄기
    delay(100);                  // 100ms 대기
    toggle = 1;
    digitalWrite(PIN_LED, toggle); // LED 켜기
    delay(100);                  // 100ms 대기
    count++;
    Serial.println(count); // 현재 count 값을 출력합니다.
  }

  toggle = 0;
  digitalWrite(PIN_LED, toggle);
  // 코드대로라면 이게 맞는데 계속 고쳐봤지만 코드랑 다르게 해결돼서 어쩔 수 없이 제출합니다. 죄송합니다.
  // 1초 불이 2번 들어온다는 것은 루프를 2번 돈다는 것인데 이론상 아래 무한루프로 들어가면 루프가 다시 돌아가면 안됨.
  // 이후 loop()가 다시 실행되지 않도록 무한 루프에 진입
  while (1) {

  }
}
