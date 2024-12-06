#include "shiftregister.h"
#include "led.h"
#include "switch.h"
#include "pillbox.h"
#include "communication.h"

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>

char* ssid = "donghoon";          // Wi-Fi 이름
char* password = "arduinotest";  // Wi-Fi 비밀번호

char* serverAddress = "172.20.10.9"; 
int serverPort = 3000;


Pillbox pillbox({3, 4, 5}, {6, 7, 8}, 9, 10, 11, 12, \
                ssid, password, serverAddress, serverPort);

void setup() {
  pillbox.initPillbox();
}

void loop() {
  pillbox.getStateFromServer();
  pillbox.updateBoxState();
  pillbox.checkBoxStateChanged();
  pillbox.sendNewIntakeToServer(1, "green");

  delay(1000); // 디바운싱 대기
}