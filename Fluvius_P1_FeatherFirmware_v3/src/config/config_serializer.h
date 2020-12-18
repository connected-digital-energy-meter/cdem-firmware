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

      // For some reason compiler does not select correct method when overloading
      static size_t serialize_primitive(char * buffer, int value);
      static size_t serialize_primitive(char * buffer, long value);

      static size_t deserialize_primitive(char * buffer, int * value);
      static size_t deserialize_primitive(char * buffer, long * value);
  };

};