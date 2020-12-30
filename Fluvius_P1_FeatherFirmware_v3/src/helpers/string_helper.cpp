#include "string_helper.h"
#include <cctype>

namespace SmartMeter {

  bool StringHelper::is_number(String value) {
    if (value == "") return false;

    unsigned int start = 0;
    if (value[0] == '-' || value[0] == '+') start = 1;

    for (unsigned int i = start; i < value.length(); i++) {
      if (!std::isdigit(value[i])) return false;
    }

    return true;
  }

};