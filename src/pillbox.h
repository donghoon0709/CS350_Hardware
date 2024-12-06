#ifndef PILLBOX_H
#define PILLBOX_H

#include "led.h"
#include "shiftregister.h"
#include "switch.h"
#include "communication.h"

#include <Arduino_JSON.h>

enum BoxState {
  BOX_OPEN,
  BOX_CLOSED,
  BOX_EMPTY
};

class Pillbox {
  private:
    ReedSwitch* switches[4];
    ShiftRegister* registers[2];
    BoxState boxState[4];
    BoxState lastBoxState[4];

    bool isTaken[4];
    Communication* com;
    
    int openingTimeCount[4];

  public:
    Pillbox(const ShiftRegisterPins r1pins, \
            const ShiftRegisterPins r2pins, \
            int s1, int s2, int s3, int s4, \
            char* ssid, char* password,     \
            char* serverAddress, int serverPort) {
      registers[0] = new ShiftRegister(r1pins);
      registers[1] = new ShiftRegister(r2pins);

      switches[0] = new ReedSwitch(s1);
      switches[1] = new ReedSwitch(s2);
      switches[2] = new ReedSwitch(s3);
      switches[3] = new ReedSwitch(s4);

      com = new Communication(ssid, password, serverAddress, serverPort);

      for (int i = 0; i < 4; ++i) {
        boxState[i] = BOX_EMPTY;
        lastBoxState[i] = BOX_EMPTY;
        isTaken[i] = false;
        openingTimeCount[i] = 0;
      }
    }

    ~Pillbox () {
      delete registers[0];
      delete registers[1];

      delete switches[0];
      delete switches[1];
      delete switches[2];
      delete switches[3];

      delete com;
    }
    void initPillbox(void) {
      com->checkSerialCommunication();
      com->checkWiFiConnection();
      com->checkServerConnection();
    }
 
    BoxState getBoxState (int idx) {
      return boxState[idx];
    }

    void getStateFromServer() {
      JSONVar state = com->getRequest("/api/hardware/change-state");

      for (int i = 0; i < 4; ++i) {
        String stateStr = state["state"][i];

        if (stateStr.equals("empty")) continue;
        if (stateStr.equals("red")){
          if (lastBoxState[i] == BOX_EMPTY) boxState[i] = BOX_CLOSED; // 비어있는 상자에 새로운 약 할당
          isTaken[i] = false;
        }
        else boxState[i] = BOX_OPEN;
        
      }
      Serial.println("");
      
    }
    
    void updateBoxState () {
      for (int i = 0; i < 4; ++i) {
        int registerIndex = i / 2;
        int ledIndex = i % 2;

        registers[registerIndex]->setLEDColor(ledIndex, isTaken[i] ? GREEN : RED);

        if (switches[i]->getSwitchState() == NOMAGNET) boxState[i] = BOX_OPEN;
        else boxState[i] = BOX_CLOSED;
        Serial.print(boxState[i]);
      }
    }

    void checkBoxStateChanged () {
      
      for (int i = 0; i < 4; ++i) {
        if (i != 2) continue;
        if (lastBoxState[i] == BOX_CLOSED) {
          if (boxState[i] == BOX_OPEN) handleOpenBox(i);
          else continue;
        }
        else if (lastBoxState[i] == BOX_OPEN) {
          if (boxState[i] == BOX_CLOSED) handleCloseBox(i);
          else handleKeepOpeningBox(i);
        }
      }
    }

    void handleOpenBox(int boxIndex) {  // close -> open
      int registerIndex = boxIndex / 2;
      int ledIndex = boxIndex % 2;

      if (isTaken[boxIndex]) {      // 약을 먹은 상태에서 상자를 열 때,
        registers[registerIndex]->setLEDBlink(ledIndex, GREEN);
      }
      else {  // 약을 먹지 않은 상태에서 상자를 열 때,
        registers[registerIndex]->setLEDColor(ledIndex, YELLOW);
      }
      lastBoxState[boxIndex] = BOX_OPEN;
    }

    void handleCloseBox(int boxIndex){
      int registerIndex = boxIndex / 2;
      int ledIndex = boxIndex % 2;

      registers[registerIndex]->setLEDColor(ledIndex, GREEN);
      lastBoxState[boxIndex] = BOX_CLOSED;

      isTaken[boxIndex] = true;
      openingTimeCount[boxIndex] = 0;

      sendNewIntakeToServer(boxIndex, "green");
    }
    
    void handleKeepOpeningBox(int boxIndex) {
      int registerIndex = boxIndex / 2;
      int ledIndex = boxIndex % 2;

      openingTimeCount[boxIndex]++;
      if (openingTimeCount[boxIndex] > 5) {
        registers[registerIndex]->setLEDBlink(ledIndex, YELLOW);

        sendKeepOpeningStateToServer();
      }
      lastBoxState[boxIndex] = BOX_OPEN;
    }

    void sendNewIntakeToServer(int index, String newStatus) {
      String jsonData = "{\"pillboxIndex\": " + String(index+1) + 
                    ", \"newStatus\": \"" + newStatus + "\"}";
      com->postRequest(jsonData, "/api/hardware/update");
    }

    void sendKeepOpeningStateToServer() {
      String jsonData = "{\"isTaken\": [";
      for (int i = 0; i < 4; i++) {
        jsonData += openingTimeCount[i] > 50 ? "true" : "false";
        if (i < 3) jsonData += ",";
      }
      jsonData += "]}";
      String warningMessage = "{\"message\": \"Warning: Pillbox #" + String(index+1) + "is OPEN" +"\"}"
      com->postRequest("{\"message\": \"warning\"}" , "/api/notifications");
    }
};

#endif // PILLBOX_H