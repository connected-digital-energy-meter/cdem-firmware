#pragma once

// TODO - Bad name for this class but cant think of better one at the moment
// TODO - This class allows user to configure system

#include <Arduino.h>
#include "../config/configuration.h"

namespace SmartMeter {

  class BootConfig {

    public:
      BootConfig(Configuration current_config, HardwareSerial * userSerial);

    public:
      //functions
      void Enable_Bootmenu(void);

    private:  
      void Save_Settings(void);
      int Menu_Selection(void);
      void Menu_Action(void);  
      String Request_Input(String info, String current_value);

    public:
      //variables
      Configuration current_config;
      bool showboot_config;
      HardwareSerial * userSerial;
  };

};