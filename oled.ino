bool oled_is_ON = true;

void oled_setup() {
  // Wait for display
  delay(500);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {};  // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();
  display.display();
}

void oled_handle() {
  if (oled_flag) {
    static bool isFirst = true;
    static uint32_t preMillis = millis();
    if (isFirst || (30000 <= (millis() - preMillis))) {
      preMillis = millis();
      isFirst = false;

      digitalWrite(LED_BUILTIN, LOW);

      display.clearDisplay();  // Clear the buffer

      String info;
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0, 0);
      info = "Temp:" + String(gotTemperature) + "C";
      display.println(info);
      display.setCursor(0, 10);
      info = "RH:" + String(gotHumidity) + "%";
      display.println(info);
      display.setCursor(0, 20);
      info = "AQI:" + ENS_parameters_convert(gotAQI);
      display.println(info);
      display.setCursor(0, 30);
      info = "TVOC:" + TVOC_parameters_convert(gotTVOC);
      display.println(info);
      display.setCursor(0, 40);
      info = "eCO2:" + eCO2_parameters_convert(gotECO2);
      display.println(info);
      display.setCursor(0, 50);
      display.print("STA IP:" + WiFi.localIP().toString());
      display.display();

      oled_on();  // 点灯

      digitalWrite(LED_BUILTIN, HIGH);
    }
  } else if (oled_is_ON) oled_off();  // 消灯
}

void oled_on() {
  oled_is_ON = true;
  display.ssd1306_command(SSD1306_DISPLAYON);
}

void oled_off() {
  oled_is_ON = false;
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}
