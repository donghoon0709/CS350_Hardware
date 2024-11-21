#include <iostream>
#include "led.h"
using namespace std;

int main() {
    RGBLED myLED; // Create an LED object
    int input;

    cout << "Enter the LED color (0 for RED, 1 for GREEN, 2 for YELLOW): ";
    cin >> input;

    // Validate input and set the LED color
    if (input >= 0 && input <= 2) {
        myLED.setColor(static_cast<LEDColor>(input));
        LEDColor color = myLED.getColor();
        switch (color) {
            case RED:
                cout << "Current LED color: RED" << endl;
                break;
            case GREEN:
                cout << "Current LED color: GREEN" << endl;
                break;
            case YELLOW:
                cout << "Current LED color: YELLOW" << endl;
                break;
            default:
                cout << "Unknown LED color" << endl;
        }
    } 
    else {
        cout << "Invalid input. Please enter 0, 1, or 2." << endl;
    }

    return 0;
}
