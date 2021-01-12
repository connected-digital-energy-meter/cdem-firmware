#include "effect.h"

namespace SmartMeter {

  namespace Effects {

    Effect::Effect(RgbLed * led) {
      this->_led = led;
    }

    void Effect::start(void) {
      this->led()->clear();
    }

    void Effect::stop(void) {
      this->led()->clear();
    }

    RgbLed * Effect::led(void) {
      return this->_led;
    }

  };

};