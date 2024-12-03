#ifndef PILLBOX_H
#define PILLBOX_H

#include "led.h"
#include "shiftregister.h"
#include "switch.h"

enum BoxState {
  OPEN,
  CLOSED
};

class Pillbox {
  private:
    ReedSwitch* switches[4];
    ShiftRegister* registers[2];
    BoxState boxState[4];
    BoxState lastBoxState[4];

  public:
    Pillbox(const ShiftRegisterPins r1pins, \
            const ShiftRegisterPins r2pins, \
            int s1, int s2, int s3, int s4) {
      registers[0] = new ShiftRegister(r1pins);
      registers[1] = new ShiftRegister(r2pins);

      switches[0] = new ReedSwitch(s1);
      switches[1] = new ReedSwitch(s2);
      switches[2] = new ReedSwitch(s3);
      switches[3] = new ReedSwitch(s4);

      for (int i = 0; i < 4; ++i) {
        boxState[i] = CLOSED;
        lastBoxState[i] = CLOSED;
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
        if (switches[i]->getSwitchState() == NOMAGNET) boxState[i] = OPEN;
        else boxState[i] = CLOSED;
      }
      checkBoxStateChanged ();
    }

    void checkBoxStateChanged () {
      for (int i = 0; i < 4; ++i) {
        if (lastBoxState[i] == CLOSED) {
          if (boxState[i] == OPEN) handleOpenBox(i);
          else continue;
        }
        else if (lastBoxState[i] == OPEN) {
          if (boxState[i] == CLOSED) handleCloseBox(i);
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
        lastBoxState[boxIndex] = OPEN;
      }
      else if (current_led == RED) {  // red(close)일 때 open => yellow
        registers[registerIndex]->setLEDColor(ledIndex, YELLOW);
        lastBoxState[boxIndex] = OPEN;
      }
    }
    void handleCloseBox(int boxIndex){
      int registerIndex = boxIndex / 2;
      int ledIndex = boxIndex % 2;

      registers[registerIndex]->setLEDColor(ledIndex, GREEN);
      lastBoxState[boxIndex] = CLOSED;

      // 앱에 복용 완료 알림 보내기
    }
    void handleKeepOpeningBox(int boxIndex){
      LEDColor current_led = registers[registerIndex]->getLEDcolor(ledIndex);
      
      if (current_led == GREEN) {     // green(open)일 때 open => 초록 점멸
        registers[registerIndex]->setLEDBlink(ledIndex, GREEN);
      }
      // yellow일 때 open => continue
    }
};

#endif // PILLBOX_H