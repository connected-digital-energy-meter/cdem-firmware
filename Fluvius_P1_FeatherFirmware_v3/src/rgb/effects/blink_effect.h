#pragma once

#include "effect.h"

namespace SmartMeter {

  namespace Effects {

    class BlinkEffect : public Effect {

      public:
        BlinkEffect(RgbLed * led, Color color, unsigned int delayMs);

      public:
        virtual void start(void) override;
        virtual void output(unsigned int deltaMilliseconds) override;

      private:
        Color color;
        bool isOn = false;
        unsigned int delayMs;
        unsigned int currentDelay = 0;
    };

  };

};