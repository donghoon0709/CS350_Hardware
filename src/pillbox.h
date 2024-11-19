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
      
    }

};

#endif // PILLBOX_H