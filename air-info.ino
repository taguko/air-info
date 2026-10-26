/*
 Air Info
*/

// バス
#include <Wire.h>
#include <SPI.h>
// センサー
#include <SparkFun_Qwiic_Humidity_AHT20.h>
#include <SparkFun_ENS160.h>
// ネットワーク
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
// OLED
#include "oled.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const uint8_t button = D0;
bool oled_flag = true;

// センサー値取得
int
  gotAQI,
  gotTVOC,
  gotECO2;
float
  gotTemperature,
  gotHumidity;

// インスタンスの作成
AHT20 AHT;
SparkFun_ENS160 ENS;
WebServer server(80);  // ポート80を開く
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  setCpuFrequencyMhz(80);
  pinMode(button, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);  // シリアルポートを115200bpsで開く
  oled_setup();
  sensor_setup();
  web_setup();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  static uint32_t preMillis = millis();

  // ボタンが押されたら点灯
  if (!oled_flag && button_push()) {
    preMillis = millis();
    oled_flag = true;
  }
  // 30秒以上経過で消灯
  if (30000 <= (millis() - preMillis)) {
    oled_flag = false;
  }

  sensor_handle();        // センサー処理
  oled_handle();          // OLED処理
  server.handleClient();  // サーバ処理
}

bool button_push() {
  static bool before = HIGH;
  bool now = digitalRead(button);
  if (before != now) {
    before = now;
    if (now == LOW) return true;
  }
  return false;
}
