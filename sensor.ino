/* 
  AHT21:Temp, Humi
  ENS160:AQI, TVOC, eCO2
*/

// キャリブレーション
const float tempCali = -6;
const float humiCali = 10;

// パラメータ（データシートより）
const int eCO2_parameters[6] = { 0, 0, 600, 800, 1000, 1500 };  // ppm
const int TVOC_parameters[6] = { 0, 0, 65, 220, 650, 2200 };    // ppb

void sensor_setup() {
  Wire.begin();  // I2Cの開始

  // AHT21の接続テスト
  if (!AHT.begin()) {
    Serial.println("AHT21 CONNECTION FAILED");
    while (true) {};
  }
  Serial.println("AHT21 CONNECTION SUCCESS");

  // ENS160の接続テスト
  if (!ENS.begin()) {
    Serial.println("ENS160 CONNECTION FAILED");
    while (true) {};
  }
  Serial.println("ENS160 CONNECTION SUCCESS");

  // ENS160の設定をリセット
  if (!ENS.setOperatingMode(SFE_ENS160_RESET)) {
    Serial.println("ENS160 RESET FAILED");
    while (true) {};
  }
  Serial.println("ENS160 RESET SUCCESS");
  delay(100);

  // ENS160の設定を標準動作にセット
  if (!ENS.setOperatingMode(SFE_ENS160_STANDARD)) {
    Serial.println("ENS160 SFE_ENS160_STANDARD FAILED");
    while (true) {};
  }
  Serial.println("ENS160 SFE_ENS160_STANDARD SUCCESS");
}

void sensor_handle() {
  static uint32_t preMillis = millis();
  static bool isFirst = true;
  if (isFirst || 30000 < (millis() - preMillis)) {
    preMillis = millis();
    isFirst = false;

    // センサー値取得
    gotAQI = ENS.getAQI(),
    gotTVOC = ENS.getTVOC(),
    gotECO2 = ENS.getECO2();
    gotTemperature = AHT.getTemperature() + tempCali,
    gotHumidity = AHT.getHumidity() + humiCali;
  }
  ENS_compensation();  // ATH2x→ENS160 温度・湿度を渡す
}

// ATH2x→ENS160 温度・湿度を渡す
void ENS_compensation() {
  static uint32_t preMillis = millis();
  static bool isFirst = true;

  //5分毎
  if (isFirst || 300000 < (millis() - preMillis)) {
    preMillis = millis();
    isFirst = false;
    ENS.setTempCompensationCelsius(AHT.getTemperature() + tempCali);
    ENS.setRHCompensationFloat(AHT.getHumidity() + humiCali);
    Serial.println("ENS160 compensationed");
    delay(500);
  }
}

String TVOC_parameters_convert(int n) {
  int result = 1;
  for (int i = 2; i < 6; i++) {
    if (TVOC_parameters[i] < n) result = i;
  }
  return ENS_parameters_convert(result);
}

String eCO2_parameters_convert(int n) {
  int result = 1;
  for (int i = 1; i < 6; i++) {
    if (eCO2_parameters[i] < n) result = i;
  }
  return ENS_parameters_convert(result);
}

String ENS_parameters_convert(int n) {
  const char* parameters[6] = {
    "",
    "Excellent",
    "Good",
    "Fair"
    "Moderate",
    "Poor",
    "Unhealthy"
  };
  return parameters[n];
}
