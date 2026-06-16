#include <Arduino.h>
#include <SPIFFS.h> 
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

const char* ssid = "BELL601";
const char* password = "239FA1322CDE";
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnection complete!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // SPIFFS 초기화 - 실패하면 시리얼 모니터에 에러 출력
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Initialization Failed!");
    return;
  }
  Serial.println("SPIFFS Initialization Successful!");

    // 텍스트 대신 data/index.html 파일을 브라우저에 보내줌
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
      });

    // 서버 시작 - 이 줄 이후부터 브라우저 접속 가능
    server.begin();
    Serial.println("web server started");
}

void loop() {

}