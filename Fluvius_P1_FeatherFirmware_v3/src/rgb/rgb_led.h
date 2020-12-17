/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * Class to controll the RGB led's on the PCB
 *
*/
#pragma once

#include "color.h"

namespace SmartMeter {
  class RgbLed {

    public:
      RgbLed(int pinRed, int pinGreen, int pinBlue, int firstChannel);

    public:
      void color(Color color);
      void on(void);
      void off(void);
      void clear(void);

    private:
      void init_channel(int pin, int channel);
      void write_color(Color color);

    private:
      int _pinRed;
      int _pinGreen;
      int _pinBlue;
      int _firstChannel;
      Color _color;

  };
};
