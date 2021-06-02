#pragma once

#include <Arduino.h>
#include "../config/configuration.h"
#include "../config/helpers/user_config_input_helper.h"

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

    private:
      void configure_mqtt(void);
      int request_mqtt_menu_selection(void);

    private:
      void configure_meter(void);
      int request_meter_menu_selection(void);

    private:
      void restore_factory_defaults(void);

    private:
      void configure_wizard(void);

    public:
      Configuration originalConfig;
      Configuration newConfig;
      HardwareSerial * userSerial;
      UserConfigInputHelper userConfigInputHelper;
  };

};