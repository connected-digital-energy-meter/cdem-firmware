#include "boot_manager.h"
#include "../../hardware.h"
#include "boot_config.h"
#include "boot_wizard.h"

namespace SmartMeter {

  BootManager::BootManager(void) {
    pinMode(BOOT_PIN, INPUT_PULLUP);
  }

  Configuration BootManager::boot(void) {
    // Loading the EEPROM or factory default configuration settings 
    SerialDebug.println("Loading configuration ...");
    if (configManager.load_configuration()) {
      SerialDebug.println("Loaded existing configuration");
    } else {
      SerialDebug.println("No existing configuration could be loaded");
      SerialDebug.println("Booting configuration wizard ...");
      configManager.factory_default();
      
      BootWizard bootWizard(configManager.current_config(), &SerialDebug);
      Configuration config = bootWizard.RunWizard();

      // This point will only be reached once the user has stepped through
      // the configuration wizard. So here we have to save.
      SerialDebug.println("Saving device configuration ...");
      configManager.current_config(config);
      save_config();
      delay(1000);
    }

    SerialDebug.println("Hold the touch if you wish to boot into boot menu");
    SerialDebug.print("Booting in");
    for (int i = BOOT_MENU_TIME; i >= 0; i--) {
      SerialDebug.print(" ... " + String(i));
      if (touchRead(BOOT_PIN) < 40) {
        SerialDebug.println("");
        show_boot_menu();
        break;
      }
      delay(1000);
    }

    SerialDebug.println("");

    return configManager.current_config();
  }

  void BootManager::show_boot_menu(void) {
    BootConfig bootConfig(configManager.current_config(), &SerialDebug);
    Configuration config = bootConfig.enable_boot_menu();

    if (config != this->configManager.current_config()) {
      SerialDebug.println("Device configuration has changed. Saving it ...");
      configManager.current_config(config);
      save_config();
    } else {
      SerialDebug.println("Device configuration is not altered. No need for saving it.");
    }
  }

  void BootManager::save_config(void) {
    if (configManager.save_configuration()) {
      SerialDebug.println("Successfully saved configuration");
    } else {
      SerialDebug.println("Something went wrong. Could not save configuration.");
    }
  }

};