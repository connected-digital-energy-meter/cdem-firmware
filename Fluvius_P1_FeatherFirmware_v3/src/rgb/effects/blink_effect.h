#pragma once

#include "effect.h"

namespace SmartMeter {

  namespace Effects {

    class BlinkEffect : public Effect {

      public:
        BlinkEffect(RgbLed * led, Color color, unsigned int delayMs, unsigned int startDelay=0);

      public:
        virtual void start(void) override;
        virtual void stop(void) override;
        virtual void output(unsigned int deltaMilliseconds) override;

      private:
        Color color;
        bool isOn = false;
        unsigned int delayMs;
        unsigned int currentDelay = 0;
        unsigned int startDelay = 0;
        bool started = false;
    };

  };

};