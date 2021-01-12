#include "boot_manager.h"
#include "../../hardware.h"
#include "boot_config.h"
#include "boot_wizard.h"
#include "../helpers/serial_helper.h"

namespace SmartMeter {

  BootManager::BootManager(DeviceStatus * deviceStatus) {
    this->deviceStatus = deviceStatus;
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
      deviceStatus->config_wizard();
      
      // run config wizzard
      BootWizard bootWizard(configManager.current_config(), &SerialDebug);
      bool save=false;
      String input = "";
      Configuration config;
      do{
        config = bootWizard.RunWizard();
        SerialDebug.println(config.to_string());
        do {
          SerialDebug.println("Are you sure you wish to save this configuration? [y/N]");
          input = SerialHelper::read_line(&SerialDebug);
          input.toLowerCase();
          SerialDebug.println("");
        } while (input != "y" && input != "n" && input != "");

        if (input == "y") {
          save=true;
        }

      } while(!save);

      // This point will only be reached once the user has stepped through
      // the configuration wizard. So here we have to save.
      SerialDebug.println("Saving device configuration ...");
      configManager.current_config(config);
      save_config();
      delay(1000);
    }

    deviceStatus->booting();

    SerialDebug.println("Hold the touch if you wish to boot into boot menu");
    SerialDebug.print("Booting in");
    for (int i = BOOT_MENU_TIME; i >= 0; i--) {
      SerialDebug.print(" ... " + String(i));
      if (touchRead(BOOT_PIN) < TOUCH_SENSITIVITY) {
        deviceStatus->boot_menu();
        SerialDebug.println("");
        show_boot_menu();
        break;
      }
      delay(1000);
    }

    SerialDebug.println("");
    deviceStatus->booting();

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