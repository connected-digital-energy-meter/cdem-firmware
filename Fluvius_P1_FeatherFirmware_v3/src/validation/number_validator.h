#pragma once

#include "validator.h"

namespace SmartMeter {

  class NumberValidator : public Validator {

    public:
      NumberValidator(long minimum, long maximum);

    public:
      virtual bool is_valid(String value);

    private:
      long minimum;
      long maximum; 
  };

};