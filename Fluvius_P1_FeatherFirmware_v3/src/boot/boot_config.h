#pragma once

// TODO - Bad name for this class but cant think of better one at the moment
// TODO - This class allows user to configure system

#include <Arduino.h>
#include "../config/configuration.h"

namespace SmartMeter {

  class BootConfig {

    public:
      BootConfig(Configuration currentConfig, HardwareSerial * userSerial);

    public:
      Configuration enable_boot_menu(void);  

    private:
      int request_menu_selection(void);

    private:
      void reset_new_config(void);

    private:
      void configure_network(void);
      int request_network_menu_selection(void);
      void configure_wifi_ssid(void);
      void configure_wifi_password(void);
      void configure_static_ip(void);
      void configure_subnet_mask(void);
      void configure_default_gateway(void);

    private:
      void configure_mqtt(void);
      int request_mqtt_menu_selection(void);
      void configure_mqtt_broker(void);
      void configure_mqtt_broker_port(void);
      void configure_mqtt_topic(void);

    private:
      void configure_meter(void);
      int request_meter_menu_selection(void);
      void configure_device_read_period(void);

    private:
      void restore_factory_defaults(void);

    private:
      String request_input(String info, String current_value);

    public:
      //variables
      Configuration originalConfig;
      Configuration newConfig;
      HardwareSerial * userSerial;
  };

};