#pragma once

#include <Arduino.h>
#include "../configuration.h"
#include "../../validation/validator.h"

namespace SmartMeter {

  class UserConfigInputHelper {

    public:
      UserConfigInputHelper(HardwareSerial * userSerial);

    public:
      void configure_wifi_ssid(Configuration * config);
      void configure_wifi_password(Configuration * config);
      void configure_static_ip(Configuration * config);
      void configure_subnet_mask(Configuration * config);
      void configure_default_gateway(Configuration * config);

    public:
      void configure_mqtt_broker(Configuration * config);
      void configure_mqtt_broker_port(Configuration * config);
      void configure_mqtt_topic(Configuration * config);

    public:
      void configure_device_read_period(Configuration * config);

    private:
      String request_input(String info, String current_value, Validator * validator=nullptr);

    private:
      HardwareSerial * userSerial;
  };

};