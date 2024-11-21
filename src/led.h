#ifndef LED_H
#define LED_H

enum LEDColor {
  RED,
  GREEN,
  YELLOW, 
  BLINK_GREEN
};

class RGBLED {
  private:
    LEDColor currentColor;

  public:
    void setColor (LEDColor color) {
      currentColor = color;
    }

    LEDColor getColor () {
      return currentColor;
    }
};

#endif  // LED_H