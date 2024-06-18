#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

//  Wi-Fi
const char* ssid = "CALM LAND 5G";
const char* password = "luckyseven";

// Tạo một đối tượng WebServer (port 80)
WebServer server(80);

// GPIO pin mà LED được kết nối
const int ledPin = 2;

void handleToggle() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));
    bool state = doc["state"];
    digitalWrite(ledPin, state ? HIGH : LOW);
    server.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\"}");
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Kết nối với Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.println(WiFi.localIP());

  // Định nghĩa các route cho web server
  server.on("/toggle", HTTP_POST, handleToggle);

  // Bắt đầu server
  server.begin();
}

void loop() {
  server.handleClient();
}