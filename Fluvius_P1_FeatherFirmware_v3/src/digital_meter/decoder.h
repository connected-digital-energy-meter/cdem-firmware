#pragma once

#include "datagram.h"

namespace SmartMeter {

  class Decoder {

    public:
      static Datagram decode(char* buffer, size_t bufferlength);

    private:
      static double parse_data_value(char* _buffer, char* key, int datablock);

  };

};