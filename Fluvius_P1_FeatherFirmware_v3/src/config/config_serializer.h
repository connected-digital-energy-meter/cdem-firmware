/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * Class to handle the EEPROM config info
 *
*/
#pragma once

#include "configuration.h"

namespace SmartMeter {

  class ConfigSerializer {

    public:
      static size_t serialize(char * buffer, size_t size, Configuration * config);
      static size_t deserialize(char * buffer, size_t size, Configuration * config);

    private:
      static size_t serialize_string(char * buffer, String value);

  };

};