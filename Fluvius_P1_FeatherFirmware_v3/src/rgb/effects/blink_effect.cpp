#include "blink_effect.h"

namespace SmartMeter {

  namespace Effects {

    BlinkEffect::BlinkEffect(RgbLed * led, Color color, unsigned int delayMs)
      : Effect(led) {

      this->color = color;
      this->delayMs = delayMs;
    }

    void BlinkEffect::start(void) {
      currentDelay = 0;
      isOn = false;
    }

    void BlinkEffect::output(unsigned int deltaMilliseconds) {
      currentDelay += deltaMilliseconds;

      if (currentDelay >= delayMs) {
        if (isOn) {
          this->led()->color(Color::BLACK());
        } else {
          this->led()->color(color);
        }

        isOn = !isOn;
        currentDelay = 0;
      }
    }

  };

};