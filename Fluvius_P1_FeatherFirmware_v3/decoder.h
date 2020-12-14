#pragma once

// #include <Arduino.h>
#include "datagram.h"

namespace SmartMeter {
  class Decoder {
      public:
        static Datagram decode(char* buffer, size_t bufferlength);

      private:
        static double ParseDataValue(char* _buffer, char* key, int datablock);


  };
};