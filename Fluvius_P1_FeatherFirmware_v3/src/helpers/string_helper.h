#pragma once

# include <Arduino.h>

namespace SmartMeter {

  class StringHelper {

    public:
      static bool is_number(String value);

  };

};