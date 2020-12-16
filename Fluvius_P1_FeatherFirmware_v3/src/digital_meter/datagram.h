#pragma once

#include <Arduino.h>
#include <map>
#include <vector>

namespace SmartMeter {
  class Datagram {

    public:
      void set(String key, double value);
      double get(String key);
      
    public:
      String to_string();

    public:
      std::vector<String> keys(void);

    private:
       std::map<String, double> data;

  };
};