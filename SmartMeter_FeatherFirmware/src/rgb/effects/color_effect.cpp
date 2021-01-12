#include "color_effect.h"

namespace SmartMeter {

  namespace Effects {

    ColorEffect::ColorEffect(RgbLed * led, Color color)
      : Effect(led) {

      this->color = color;
    }

    void ColorEffect::start(void) {
      this->led()->color(this->color);
    }

    void ColorEffect::output(unsigned int deltaMilliseconds) {
      // Don't need to do anything here
    }

  };

};