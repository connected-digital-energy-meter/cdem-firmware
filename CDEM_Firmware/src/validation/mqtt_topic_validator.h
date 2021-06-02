#pragma once

#include "validator.h"

namespace SmartMeter {

  class MqttTopicValidator : public Validator {

    public:
      virtual bool is_valid(String value);
  };

};