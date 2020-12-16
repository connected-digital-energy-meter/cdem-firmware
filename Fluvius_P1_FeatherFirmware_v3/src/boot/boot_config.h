#pragma once

#include "configuration.h"

namespace SmartMeter {

  class BootManager {

    public:
      BootManager(Configuration current_config);

    public:
      //functions
      Enable_Bootmenu();

    private:  
      Save_Settings();
      int Menu_Selection();
      Menu_Action();  
      String Request_input(String info, String current_value);

    public:
      //variables
      Configuration current_config;
      bool showboot_config;
  };

};