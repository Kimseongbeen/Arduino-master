unsigned long previousMillis = 0;  // 이전 시간
unsigned long interval = random(100, 200) * 100;  // 출력 간격(10~20분) 60000ms, 60s(1분)

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();  // 현재 시간

  if (currentMillis - previousMillis >= interval) {
    // 이전 출력 시간에서 출력 간격(interval) 이상 시간이 지난 경우
    previousMillis = currentMillis;  // 이전 시간을 현재 시간으로 업데이트
    Serial.println(interval);
    Serial.println("Hello, World!");  // 출력
    interval = random(100, 200) * 100;
    Serial.println(interval);
  }
}
