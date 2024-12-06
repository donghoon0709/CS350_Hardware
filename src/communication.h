#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "pillbox.h"
#include "led.h"

class Communication {
    private:
        char* ssid;
        char* password;
        char* serverAddress;
        int serverPort;

        WiFiClient wifiClient;
        HttpClient* httpClient;

    public:
        Communication(  char* ssid,\
                        char* password,\
                        char* serverAddress,\
                        int serverPort) {
            this->ssid = ssid;
            this->password = password;
            this->serverAddress = serverAddress;
            this->serverPort = serverPort;

            httpClient = new HttpClient(wifiClient, serverAddress, serverPort);
        }
        ~Communication() {
            delete httpClient;
        }
        void checkSerialCommunication() {
            Serial.begin(9600);
            while (!Serial);
            Serial.println("Serial Communication Activated");
        }

        void checkWiFiConnection() {
            Serial.print("1. Attempting to connect to Wi-Fi...");
            WiFi.begin(ssid, password);
            while (WiFi.status() != WL_CONNECTED) {
                delay(1000);
                Serial.print(".");
            }
            Serial.println("\nConnected to Wi-Fi!");
        }

        void checkServerConnection() {
            Serial.println("2. Checking server connection...");

            httpClient->beginRequest();
            httpClient->get("/ping"); // 서버의 `/ping` 엔드포인트로 요청
            httpClient->endRequest();

            int statusCode = httpClient->responseStatusCode();
            String response = httpClient->responseBody();

            if (statusCode == 200 && response == "pong") {
                Serial.println("Connected to server successfully!");
            } else {
                Serial.println("Failed to connect to server. Retrying...");
                delay(3000); // 3초 대기 후 재시도
                checkServerConnection(); // 재귀 호출로 재시도
            }
        }
        JSONVar getRequest(char* endpoint) {
            httpClient->get(endpoint);

            int statusCode = httpClient->responseStatusCode();
            String response = httpClient->responseBody();

            return JSON.parse(response);
        }
        JSONVar postRequest(String data, char* endpoint) {
            httpClient->beginRequest();
            httpClient->post(endpoint, "application/json", data);
            httpClient->endRequest();

            int statusCode = httpClient->responseStatusCode();
            String response = httpClient->responseBody();
            Serial.print("Server Response: ");
            Serial.println(response);

            return JSON.parse(response);
        }
};

#endif