#include "rgb_led.h"
#include <Arduino.h>

namespace SmartMeter {

  RgbLed::RgbLed(int pinRed, int pinGreen, int pinBlue, int firstChannel, bool inverted) {
    _pinRed = pinRed;
    _pinGreen = pinGreen;
    _pinBlue = pinBlue;
    _firstChannel = firstChannel;
    _inverted = inverted;
    
    init_channel(_pinRed, _firstChannel);
    init_channel(_pinGreen, _firstChannel+1);
    init_channel(_pinBlue, _firstChannel+2);

    clear();
  }

  void RgbLed::init_channel(int pin, int channel) {
    ledcAttachPin(pin, channel);
    ledcSetup(channel, 12000, 8);   // 12 kHz PWM, 8-bit resolution
  }

  void RgbLed::color(Color color) {
    _color = color;
    write_color(_color);
  }

  void RgbLed::on(void) {
    write_color(_color);
  }

  void RgbLed::off(void) {
    write_color(Color::BLACK());
  }

  void RgbLed::clear(void) {
    color(Color::BLACK());
  }

  void RgbLed::write_color(Color color) {
    if (_inverted) {
      ledcWrite(_firstChannel, 255-color.red());
      ledcWrite(_firstChannel+1, 255-color.green());
      ledcWrite(_firstChannel+2, 255-color.blue());
    } else {
      ledcWrite(_firstChannel, color.red());
      ledcWrite(_firstChannel+1, color.green());
      ledcWrite(_firstChannel+2, color.blue());
    }
  }

};
