#pragma once

#include <Arduino.h>
#include "../config/configuration.h"

namespace SmartMeter {

  class BootWizard {

    public:
      BootWizard(Configuration currentConfig, HardwareSerial * userSerial);

    public:
      Configuration RunWizard();  

    private:
      void reset_new_config(void);  
    
    public:
      // the use of original and new config is only relevant is we put a option in the wizard not to save changes.
      Configuration originalConfig;
      Configuration newConfig;
      HardwareSerial * userSerial;
  };

};