#include "config_manager.h"

#include "config_serializer.h"
#include <EEPROM.h>

// TODO - Factory reset should write the initial
// default config to the EEPROM

namespace SmartMeter {

  ConfigManager::ConfigManager(void) {
    EEPROM.begin(128);    // Define EEPROM SIZE - Refactor
  }

  bool ConfigManager::load_configuration(void) {
    char buffer[128];

    // Read identifier
    if (EEPROM.read(0) != 0xDE || EEPROM.read(1) != 0xBA) {
      return false;
    }

    // Read config size
    int offset = 2;
    int configSize = (EEPROM.read(offset) << 8) + EEPROM.read(offset+1);
    for (int i = 0; i < configSize; i++) {
      buffer[i] = EEPROM.read(offset+i);
    }

    return ConfigSerializer::deserialize(buffer, 128, &_currentConfig);
  }

  bool ConfigManager::save_configuration(void) {
    char buffer[128];
    size_t length = ConfigSerializer::serialize(buffer, 128, &_currentConfig);

    // Write identifier so we know a config is present in EEPROM
    EEPROM.write(0, 0xDE);
    EEPROM.write(1, 0xBA);

    int offset = 2;
    for (int i = 0; i < length; i++) {
      EEPROM.write(offset+i, buffer[i]);
    }
    return EEPROM.commit();
  }

  Configuration * ConfigManager::current_config(void) {
    return &_currentConfig;
  }

};