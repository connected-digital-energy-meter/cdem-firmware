/*
 * VIVES college Research
 *
 * Author Ronny Mees & Nico De Witte
 *
 * Class to decode the P1 telegram to data
 *
*/
#pragma once

#include "datagram.h"

namespace SmartMeter {

  class Decoder {

    public:
      static Datagram decode(char* buffer, size_t bufferlength);

    private:
      static double parse_data_value(char* _buffer, const char* key, int datablock);

  };

};