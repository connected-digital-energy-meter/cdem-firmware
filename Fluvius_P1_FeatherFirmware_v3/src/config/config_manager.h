#pragma once

#include "configuration.h"

namespace SmartMeter {

  class ConfigManager {

    public:
      ConfigManager(void);

    public:
      bool load_configuration(void);
      bool save_configuration(void);

    public:
      Configuration * current_config(void);

    private:
      Configuration _currentConfig;

  };

};