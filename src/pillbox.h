#ifndef PILLBOX_H
#define PILLBOX_H

#include "led.h"
#include "shiftregister.h"
#include "switch.h"

enum BoxState {
  BOX_OPEN,
  BOX_CLOSED
};

class Pillbox {
  private:
    ReedSwitch* switches[4];
    ShiftRegister* registers[2];
    BoxState boxState[4];
    BoxState lastBoxState[4];

    bool isTaken[4];
    Communication& com;

  public:
    Pillbox(const ShiftRegisterPins r1pins, \
            const ShiftRegisterPins r2pins, \
            int s1, int s2, int s3, int s4,
            Communication& com) {
      registers[0] = new ShiftRegister(r1pins);
      registers[1] = new ShiftRegister(r2pins);

      switches[0] = new ReedSwitch(s1);
      switches[1] = new ReedSwitch(s2);
      switches[2] = new ReedSwitch(s3);
      switches[3] = new ReedSwitch(s4);

      this->com = com;

      for (int i = 0; i < 4; ++i) {
        boxState[i] = BOX_CLOSED;
        lastBoxState[i] = BOX_CLOSED;
        isTaken[i] = false;
      }
    }

    ~Pillbox () {
      delete registers[0];
      delete registers[1];

      delete switches[0];
      delete switches[1];
      delete switches[2];
      delete switches[3];
    }

    BoxState getBoxState (int idx) {
      return boxState[idx];
    }

    void updateBoxState () {
      for (int i = 0; i < 4; ++i) {
        if (switches[i]->getSwitchState() == NOMAGNET) boxState[i] = BOX_OPEN;
        else boxState[i] = BOX_CLOSED;
      }
    }

    void checkBoxStateChanged () {
      for (int i = 0; i < 4; ++i) {
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

      LEDColor current_led = registers[registerIndex]->getLEDcolor(ledIndex);

      if (current_led == GREEN) {      // green(close)일 때 open => 초록 점멸
        registers[registerIndex]->setLEDBlink(ledIndex, GREEN);
        lastBoxState[boxIndex] = BOX_OPEN;
      }
      else if (current_led == RED) {  // red(close)일 때 open => yellow
        registers[registerIndex]->setLEDColor(ledIndex, YELLOW);
        lastBoxState[boxIndex] = BOX_OPEN;
      }
      if (isTaken[boxIndex]) {
        registers[registerIndex]->setLEDColor(ledIndex, GREEN);
      }
      else {
        registers[registerIndex]->setLEDColor(ledIndex, YELLOW);
      }
      lastBoxState[boxIndex] = BOX_OPEN;
    }
    void handleCloseBox(int boxIndex){
      int registerIndex = boxIndex / 2;
      int ledIndex = boxIndex % 2;

      registers[registerIndex]->setLEDColor(ledIndex, GREEN);
      lastBoxState[boxIndex] = BOX_CLOSED;
    }
    void handleKeepOpeningBox(int boxIndex) {
      Serial.println("keep opening");
    }
      /*
      1. LED 색 노란색 유지
      */
};

#endif // PILLBOX_H