#pragma once

#include "../config/config_manager.h"
#include "../config/configuration.h"

// BootManager takes care of the whole boot process
// and delivers a valid usable configuration to the
// main system

namespace SmartMeter {

  class BootManager {

    public:
      Configuration boot(void);

    private:
      void show_boot_menu(void);

    private:
      ConfigManager configManager;

  };

};