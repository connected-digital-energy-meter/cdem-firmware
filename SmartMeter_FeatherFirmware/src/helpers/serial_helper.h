#pragma once

# include <Arduino.h>

namespace SmartMeter {

  class SerialHelper {

    public:
      // Blocking
      static String read_line(HardwareSerial * serial);

  };

};