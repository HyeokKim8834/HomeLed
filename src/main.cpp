#include <Arduino.h>
#include <SPIFFS.h> 
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 13      // SK6812 데이터 핀 연결될 ESP32 GPIO 번호
#define NUM_LEDS 60     // LED 개수 (나중에 실제 길이에 맞게 조정)

// NEO_GRBW: 데이터 순서가 G-R-B-W (스펙시트에서 확인한 색상 순서)
// NEO_KHZ800: SK6812가 사용하는 통신 속도 (800kHz)
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

const char* ssid = "BELL601";
const char* password = "239FA1322CDE";
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  strip.begin();   // LED 스트립 통신 시작
  strip.show();    // 모든 LED를 꺼진 상태로 초기화 (초기 노이즈 방지)
  
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

      server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
      String color = "";
      String brightness = "";
      
      if (request->hasParam("color")) {
        color = request->getParam("color")->value();
      }
      if (request->hasParam("brightness")) {
        brightness = request->getParam("brightness")->value();
      } 

      Serial.print("Color: " + color + ", Brightness: " + brightness);
      request->send(200, "text/plain", "Color: " + color + ", Brightness: " + brightness);

      // HTML color input은 "#ff0000" 형식으로 옴 - # 떼고 16진수 파싱해야 함
      // strtol(문자열, NULL, 16) = 16진수 문자열을 숫자로 변환
      long colorValue = strtol(color.substring(1).c_str(), NULL, 16);
      uint8_t r = (colorValue >> 16) & 0xFF;  // 앞 8비트 = Red
      uint8_t g = (colorValue >> 8) & 0xFF;   // 중간 8비트 = Green
      uint8_t b = colorValue & 0xFF;          // 뒤 8비트 = Blue

      int brightVal = brightness.toInt();  // "50" 같은 문자열을 숫자로 변환
      strip.setBrightness(map(brightVal, 0, 100, 0, 255));  // 슬라이더 0~100을 LED 밝기 0~255로 변환

      // 모든 LED에 같은 색 적용 (W는 0으로 - 일단 RGB만 사용)
      for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(r, g, b, 0));
      }
      strip.show();  // 실제로 LED에 신호 보내서 반영
      });
      // 서버 시작 - 이 줄 이후부터 브라우저 접속 가능
    server.begin();
    Serial.println("web server started");
} 

void loop() {

}