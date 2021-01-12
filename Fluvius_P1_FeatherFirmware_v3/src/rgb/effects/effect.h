#pragma once

#include "../rgb_led.h"
#include "../color.h"

namespace SmartMeter {

  namespace Effects {

    class Effect {

      public:
        Effect(RgbLed * led);

      public:
        virtual void start(void);
        virtual void stop(void);

      public:
        // It's like a render method
        virtual void output(unsigned int deltaMilliseconds) = 0;

      protected:
        RgbLed * led(void);

      private:
        RgbLed * _led;
    };

  };

};