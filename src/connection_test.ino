#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// Wi-Fi credentials
const char* ssid = "1209";
const char* password = "nupzuki04";

// Server details
const char* serverAddress = "192.168.0.9"; // Node.js 서버 IP
int port = 3000; // Node.js 서버 포트

WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, serverAddress, port);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  int status = WiFi.begin(ssid, password);
  while (status != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    status = WiFi.begin(ssid, password);
  }
  Serial.println("\nConnected to Wi-Fi!");
}

void loop() {
  // Send data to server
  String data = "{\"temperature\": 25, \"humidity\": 60}";
  client.beginRequest();
  client.post("/arduino-data"); // 서버의 엔드포인트
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", data.length());
  client.beginBody();
  client.print(data);
  client.endRequest();

  // Read response from server
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Response code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  delay(5000); // 5초 대기
}