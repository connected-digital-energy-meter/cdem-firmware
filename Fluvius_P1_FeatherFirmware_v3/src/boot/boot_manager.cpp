#include "boot_manager.h"
#include "../../hardware.h"
#include "boot_config.h"

namespace SmartMeter {

  Configuration BootManager::boot(void) {
    // Loading the EEPROM or factory default configuration settings 
    SerialDebug.println("Loading configuration ...");
    if (configManager.load_configuration()) {
      SerialDebug.println("Loaded existing configuration");
    } else {
      SerialDebug.println("No existing configuration could be loaded");
      SerialDebug.println("Saving factory default configuration");
      configManager.factory_default();
      if (configManager.save_configuration()) {
        SerialDebug.println("Successfully saved configuration");
      } else {
        SerialDebug.println("Something went wrong. Could not save configuration");
      }
    }

    return *configManager.current_config();
  }

  void BootManager::show_boot_menu(void) {
    BootConfig bootConfig(*configManager.current_config(), &SerialDebug);
    bootConfig.Enable_Bootmenu();
    // Get config here ?
  }

};