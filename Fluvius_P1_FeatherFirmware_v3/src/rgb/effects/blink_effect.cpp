#include "blink_effect.h"

namespace SmartMeter {

  namespace Effects {

    BlinkEffect::BlinkEffect(RgbLed * led, Color color, unsigned int delayMs, unsigned int startDelay)
      : Effect(led) {

      this->color = color;
      this->delayMs = delayMs;
      this->startDelay = startDelay;
    }

    void BlinkEffect::start(void) {
      currentDelay = 0;
      isOn = false;
      started = (startDelay == 0);
    }

    void BlinkEffect::stop(void) {
      Effect::start();
      started = false;
    }

    void BlinkEffect::output(unsigned int deltaMilliseconds) {
      currentDelay += deltaMilliseconds;

      if (!started && currentDelay >= startDelay) {
        started = true;
        currentDelay = 0;
      } else if (currentDelay >= delayMs) {
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