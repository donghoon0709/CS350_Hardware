#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <WiFiServer.h>
#include <iostream>

#include "led.h"
#include "pillbox.h"
#include "shiftregister.h"
#include "switch.h"
#include "arduino_secrets.h"

// WiFi setting
char* ssid = SECRET_SSID;        
char* password = SECRET_PW; 

// char serverAddress[] = "192.168.1.18";  // server address
// int port = 3000;

int status = WL_IDLE_STATUS;
WiFiServer server(80);
// WiFiClient wifi;
// HttpClient client = HttpClient(wifi, serverAddress, port);

Pillbox pillbox({8, 9, 10}, {11, 12, 13}, 1, 2, 3, 4);

void setup() {
    Serial.begin(9600);

    // check for the WiFi module
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("WiFi module not found!");
        while (true); // Wi-Fi 모듈이 없으면 프로그램 정지
    }

    // connect to WiFi
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);

        status = WiFi.begin(ssid, pass);
        delay(5000);
    }
    // print the SSID of the network
    Serial.println("==========");
    Serial.println("Connected to Wi-Fi!")
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);}

    server.begin();

void loop() {
    handleClientRequest();

    pillbox.updateBoxState();

    sendBoxStateToServer();
}

void handleClientRequest();

void sendBoxStateToServer();