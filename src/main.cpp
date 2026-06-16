#include <Arduino.h>
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
  
  Serial.print("WiFi 연결 중");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n연결 완료!");
  Serial.print("IP 주소: ");
  Serial.println(WiFi.localIP());

 // "text/plain; charset=utf-8" 로 인코딩 명시해줘야 한글이 깨지지 않음
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain; charset=utf-8", "ESP32 LED 서버 작동 중!");
    });

    // 서버 시작 - 이 줄 이후부터 브라우저 접속 가능
    server.begin();
    Serial.println("웹 서버 시작됨");
}

void loop() {

}