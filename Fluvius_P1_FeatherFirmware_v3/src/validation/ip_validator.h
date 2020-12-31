#pragma once

#include "validator.h"

namespace SmartMeter {

  class IpValidator : public Validator {

    public:
      virtual bool is_valid(String value);
  };

};