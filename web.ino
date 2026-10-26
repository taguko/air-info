const char *AP_SSID = "Air-Info";
const char *AP_PASSWD = "";

const char *STA_SSID = "";
const char *STA_PASSWD = "";

void IndexPage() {
  digitalWrite(LED_BUILTIN, LOW);

  String info;
  info += "Temp（温度）：" + String(gotTemperature) + "℃<br>";
  info += "RH（相対湿度）：" + String(gotHumidity) + "%<br>";
  info += "AQI（空気質指数）：" + String(gotAQI) + " (" + ENS_parameters_convert(gotAQI) + ")<br>";
  info += "TVOC（総揮発性有機化合物）：" + String(gotTVOC) + "ppb (" + TVOC_parameters_convert(gotTVOC) + ")<br>";
  info += "eCO2（等価二酸化炭素）：" + String(gotECO2) + "ppm (" + eCO2_parameters_convert(gotECO2) + ")<br>";

  String html = "<!DOCTYPE html>\n\
                <html lang=\"ja\">\n\
                  <head>\n\
		                <title>index | Air Info</title>\n\
		                <meta charset=\"UTF-8\">\n\
                    <meta http-equiv=\"refresh\" content=\"30\">\n\
                  </head>\n\
                    <body>\n\
                      <h1>Air Info</h1>\n\
                      <h2>index</h2>\n\
                      <a href=\"/generateCSV\">/generateCSV</a><br>\n"
                + info
                + "\n\
                </body>\n\
                </html>\n\
                ";

  SetCORS();
  server.send(200, "text/html", html);

  digitalWrite(LED_BUILTIN, HIGH);
}

void generateCSV() {
  digitalWrite(LED_BUILTIN, LOW);

  String csv;
  csv += "Temp[C],RH[%],AQI,TVOC[ppb],eCO2[ppm]\n";
  csv += String(gotTemperature) + "," + String(gotHumidity) + "," + String(gotAQI) + "," + String(gotTVOC) + "," + String(gotECO2) + "\n";

  SetCORS();
  server.send(200, "text/csv", csv);

  digitalWrite(LED_BUILTIN, HIGH);
}

void NotFound() {
  digitalWrite(LED_BUILTIN, LOW);
  SetCORS();
  server.send(404, "text/plain", "404 error");
  digitalWrite(LED_BUILTIN, HIGH);
}

//CORSの設定
void SetCORS() {
  //オリジン
  server.sendHeader("Access-Control-Allow-Origin", "*");
  //メゾット
  server.sendHeader("Access-Control-Allow-Methods", "GET");
}

void web_setup() {
  // STAとSoftAPを併用する
  WiFi.mode(WIFI_AP_STA);

  // STA
  WiFi.begin(STA_SSID, STA_PASSWD);
  Serial.print("Connecting to WiFi");
  uint32_t preMillis = millis();
  while ((WiFi.status() != WL_CONNECTED) && ((millis() - preMillis) < 10000)) {
    Serial.print(".");
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    Serial.print("STA IP:");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed");
    WiFi.disconnect(true);  // STAの接続をあきらめる
    WiFi.mode(WIFI_AP);     // STA失敗ならAP専用にする
  }

  // SoftAP
  WiFi.softAP(AP_SSID, AP_PASSWD);

  // mDNS
  if (MDNS.begin("air-info")) {
    Serial.println("mDNS SUCCESS");
  } else Serial.println("mDNS FAILED");

  // WebServer
  server.on("/", IndexPage);
  server.on("/generateCSV", generateCSV);
  server.onNotFound(NotFound);
  server.begin();
}
