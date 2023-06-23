/* 모듈의 맥어드레스의 값을 가져온다.*/
  
uint64_t chipId = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  chipId=ESP.getEfuseMac();
  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.printf("ESP32ChipID=%04X",(uint16_t)(chipId>>32));//print High 2bytes
  Serial.printf("%08X\r\n",(uint32_t)chipId);//print Low 4bytes.
  Serial.printf("LoRa gateway");
  Serial.printf(String(((uint16_t)(chipId>>32)),HEX)+String(((uint32_t)chipId),HEX));
  delay(3000);
}
