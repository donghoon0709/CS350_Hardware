#ifndef SWITCH_H
#define SWITCH_H

enum SwitchState {
  MAGNET,
  NOMAGNET
};

class ReedSwitch {
  private:
    int pin;

  public:
    ReedSwitch (const int p) {
      pin = p;
      pinMode(pin, INPUT_PULLUP);
    }

    SwitchState getSwitchState () { // 자석이 센서 근처에 있으면 true, 아니면 false
      if (digitalRead (pin) == LOW) return MAGNET;
      else return NOMAGNET;
    }
};

#endif // SWITCH_H