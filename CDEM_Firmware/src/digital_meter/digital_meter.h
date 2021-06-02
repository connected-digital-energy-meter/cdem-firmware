/*
 * VIVES college Research
 *
 * Author Ronny Mees & Nico De Witte
 *
 * Class to read the P1 telegram 
 *
*/
#pragma once

#include <Arduino.h>
#include "../../hardware.h"
#include "../status/device_status.h"

namespace SmartMeter {

  class DigitalMeter {

    public:
      DigitalMeter(int requestPin, DeviceStatus * deviceStatus, HardwareSerial * serial);

    public:
      void enable(void);
      void disable(void);
      void timeout(void);

    public:
      // Non-blocking function that tries to read some
      // pieces of datagram via serial port
      // return true if done
      bool read_datagram(char * buffer, size_t bufferLength);

    private:
      void clear_buffer(char * buffer, size_t length);

    private:
      HardwareSerial * serial;
      int requestPin;

      unsigned int readPointer = 0;
      bool startDetected = false;

      DeviceStatus * deviceStatus;
  };

};