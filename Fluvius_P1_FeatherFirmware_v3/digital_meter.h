#pragma once

#include <Arduino.h>

namespace SmartMeter {

  class DigitalMeter {

    public:
      DigitalMeter(int requestPin, HardwareSerial * serial, HardwareSerial * debugSerial=nullptr);

    public:
      void enable(void);
      void disable(void);

    public:
      // Non-blocking function that tries to read some
      // pieces of datagram via serial port
      // return true if done
      bool read_datagram(char * buffer, size_t bufferLength);

    private:
      void clear_buffer(char * buffer, size_t length);

    private:
      HardwareSerial * serial;
      HardwareSerial * debugSerial = nullptr;
      int requestPin;

      unsigned int readPointer = 0;
      bool startDetected = false;
      RgbLed dataLed(DATA_LED_R, DATA_LED_G, DATA_LED_B, 4);
      Color EnableMeterColor(Color::BLUE().dim(20));
      Color DisableMeterColor(Color::BLACK());

  };

};