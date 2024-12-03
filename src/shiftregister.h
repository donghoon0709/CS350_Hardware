#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include "led.h"

struct ShiftRegisterPins {
  int dataPin;
  int clockPin;
  int latchPin;
};

class ShiftRegister {
  private:
    ShiftRegisterPins pins; // 구조체로 핀 정보 관리
    RGBLED leds[2];

  public:
    // 생성자: 시프트 레지스터 핀 설정
    ShiftRegister(const ShiftRegisterPins& p) {
      pins = p;

      pinMode(pins.dataPin, OUTPUT);
      pinMode(pins.clockPin, OUTPUT);
      pinMode(pins.latchPin, OUTPUT);
    }

    // 특정 LED 색상을 설정 (0~1번 LED)
    void setLEDColor(int ledIndex, LEDColor color) {
      if (ledIndex < 0 || ledIndex >= 2) {
        Serial.println("Invalid LED index");
        return ; // 범위 검사
      }

      // LED 색상 설정
      leds[ledIndex].setColor(color);
      byte data = getShiftRegisterData();
      updateShiftRegister(data);
    }

    // 특정 LED 2회 점멸
    void setLEDBlink(int ledIndex, LEDColor color){
      if (ledIndex < 0 || ledIndex >= 2) return ;

      for (int i = 0; i < 2; i++){
        leds[ledIndex].setColor(OFF);
        byte data = getShiftRegisterData();
        updateShiftRegister(data);
        delay(500);

        leds[ledIndex].setColor(color);
        data = getShiftRegisterData();
        updateShiftRegister(data);
        delay(500);
      }
    }

    LEDColor getLEDcolor(int ledIndex) {
      return leds[ledIndex].getColor();
    }

    byte getShiftRegisterData () {
      byte result;

      result |= colorToByte(leds[0].getColor());
      result |= (colorToByte(leds[1].getColor()) >> 3);

      return (result >> 2);
    }

    byte colorToByte (const LEDColor color) {
      switch (color) {
        case RED:
          return 0b10000000;
        case GREEN:
          return 0b01000000;
        case YELLOW:
          return 0b11000000;
        case OFF:
          return 0b00000000;
      }
    }
 
    // 시프트 레지스터로 데이터 전송
    void updateShiftRegister(const byte data) {
      digitalWrite(pins.latchPin, LOW);

      shiftOut(pins.dataPin, pins.clockPin, MSBFIRST, data);  

      digitalWrite(pins.latchPin, HIGH);
    }
};

#endif // SHIFTREGISTER_H