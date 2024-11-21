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

    void updateBoxState () {
      for (int i = 0; i < 4; ++i) {
        if (switches[i]->getSwitchState() == NOMAGNET) boxState[i] = OPEN;
        else boxState[i] = CLOSED;
      }
      checkBoxStateChanged ();
    }

    void checkBoxStateChanged () {
      for (int i = 0; i < 4; ++i) {
        int registerNum = i / 2;
        int ledNum = i & 2;

        if (lastBoxState[i] == CLOSED) {
          if (boxState[i] == OPEN) handleOpenBox(registerNum, ledNum);
          else continue;
        }
        else if (lastBoxState[i] == OPEN) {
          if (boxState[i] == CLOSED) handleCloseBox(registerNum, ledNum);
          else handleKeepOpeningBox(i);
        }
      }
    }

    void handleOpenBox(int registerIndex, int ledIndex) {  // close -> open
      /*
      if (LED == green), LED 초록 점멸          // 완료 & opening
      else (LED == red), LED 노란색으로 바꾸기  // 미완료 & opening
        lastBoxState <- open
      */
      LEDColor led = registers[registerIndex]->getLEDcolor(ledIndex);
      if (led == GREEN) registers[registerIndex]->setLEDColor(ledIndex, BLINK_GREEN);
      else if (led == RED) {
        registers[registerIndex]->setLEDColor(ledIndex, YELLOW);
        lastBoxState[2*registerIndex + ledIndex] = OPEN;
      }
    }
    void handleCloseBox(int registerIndex, int ledIndex){  // open -> close
      /*
      1. LED 색 초록색으로 바꾸기
      2. lastBoxState <- close
      3. 앱에 복용 완료 알림 보내기
      */
      registers[registerIndex]->setLEDColor(ledIndex, BLINK_GREEN);
      lastBoxState[2*registerIndex + ledIndex] = CLOSED;
    }
    void handleKeepOpeningBox(int boxIndex);
      /*
      1. LED 색 노란색 유지
      */
};

#endif // PILLBOX_H