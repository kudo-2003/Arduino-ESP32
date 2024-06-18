//ESP32 MO TAT DONG CO
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

//  Wi-Fi
const char* ssid = "KuDo-2003"; // Tên của mạng Wi-Fi
const char* password = "010220031"; // Mật khẩu của mạng Wi-Fi

// GPIO pin mà LED và động cơ được kết nối
const int ledPin = 2; // Pin mà LED được kết nối
const int IN1_PIN = 12; // Pin IN1 của động cơ
const int IN2_PIN = 13; // Pin IN2 của động cơ

// Tạo một đối tượng WebServer (port 80)
WebServer server(80); // Tạo một máy chủ web trên cổng 80

//The `handleToggle()` function is responsible for handling the POST request to the "/toggle" endpoint of the web server. Here's a breakdown of what it does:
void handleToggle() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));
    bool state = doc["state"]; // Trạng thái mới của LED và động cơ
    digitalWrite(ledPin, state ? HIGH : LOW); // Đặt trạng thái mới cho LED
    if(state) {
        digitalWrite(IN1_PIN, HIGH); // Động cơ tiến
        digitalWrite(IN2_PIN, LOW);
    } else {
        digitalWrite(IN1_PIN, LOW); // Động cơ dừng
        digitalWrite(IN2_PIN, LOW);
    }
    server.send(200, "application/json", "{\"status\":\"success\"}"); // Gửi phản hồi thành công
  } else {
    server.send(400, "application/json", "{\"status\":\"error\"}"); // Gửi phản hồi lỗi
  }
}

void setup(){
    Serial.begin(115200); // Bắt đầu giao tiếp nối tiếp
    pinMode(IN1_PIN, OUTPUT); // Đặt chế độ pin IN1 là OUTPUT
    pinMode(IN2_PIN, OUTPUT); // Đặt chế độ pin IN2 là OUTPUT
    pinMode(ledPin, OUTPUT); // Đặt chế độ pin LED là OUTPUT
    digitalWrite(ledPin, LOW); // Đặt trạng thái ban đầu của LED là LOW
    WiFi.begin(ssid, password); // Kết nối đến mạng Wi-Fi
    while (WiFi.status() != WL_CONNECTED) { // Chờ đến khi kết nối thành công
        delay(1000); Serial.println("Connecting to WiFi..."); // In thông báo đang kết nối
    }
    Serial.println(WiFi.localIP()); // In địa chỉ IP cục bộ
    server.on("/toggle", HTTP_POST, handleToggle); // Đăng ký hàm xử lý khi nhận được yêu cầu POST tới "/toggle"
    server.begin(); // Bắt đầu máy chủ web
}

void loop(){ server.handleClient();} 