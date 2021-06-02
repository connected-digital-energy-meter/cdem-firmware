#pragma once

#include <Arduino.h>
#include "../config/configuration.h"
#include "../config/helpers/user_config_input_helper.h"

namespace SmartMeter {

  class BootWizard {

    public:
      BootWizard(Configuration currentConfig, HardwareSerial * userSerial);

    public:
      Configuration RunWizard(void);  

    private:
      void reset_new_config(void);  
    
    public:
      Configuration originalConfig;
      Configuration newConfig;
      HardwareSerial * userSerial;
      UserConfigInputHelper userConfigInputHelper;
  };

};