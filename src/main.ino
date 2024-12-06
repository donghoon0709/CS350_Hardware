#include "shiftregister.h"
#include "led.h"
#include "switch.h"
#include "pillbox.h"
#include "communication.h"

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>

const char* ssid = "1209";          // Wi-Fi 이름
const char* password = "nubzuki04";  // Wi-Fi 비밀번호

const char* serverAddress = "172.20.10.9"; 
const int serverPort = 3000;


Pillbox pillbox({3, 4, 5}, {6, 7, 8}, 9, 10, 11, 12, \
                ssid, password, serverAddress, serverPort);

void setup() {
}

void loop() {
  pillbox.getStateFromServer();
  pillbox.updateBoxState();
  pillbox.checkBoxStateChanged();

  delay(50); // 디바운싱 대기
}