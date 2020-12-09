#pragma once
#include <Arduino.h>

namespace SmartMeter {
  class CRCchecker {
      public:
        static bool checkCRC(char* buffer, size_t bufferlength);
      private:
        static int getposition(const char* array, size_t size, char c);  
  };
};