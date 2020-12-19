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
      //functions
      Configuration Enable_Bootmenu(void);

    private:  
      int Menu_Selection(void);
      void Menu_Action(void);  
      String Request_Input(String info, String current_value);

    private:
      void reset_new_config(void);

    public:
      //variables
      Configuration originalConfig;
      Configuration newConfig;
      bool showboot_config;
      HardwareSerial * userSerial;
      bool returnNewConfig = false;
  };

};