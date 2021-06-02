#pragma once

#include "effect.h"

namespace SmartMeter {

  namespace Effects {

    class ColorEffect : public Effect {

      public:
        ColorEffect(RgbLed * led, Color color);

      public:
        virtual void start(void) override;
        virtual void output(unsigned int deltaMilliseconds) override;

      private:
        Color color;

    };

  };

};