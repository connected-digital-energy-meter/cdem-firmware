#include "config_manager.h"

#include "config_serializer.h"
#include <EEPROM.h>
#include "factory_default_config.h"

namespace SmartMeter {

  bool ConfigManager::initialize_eeprom(void) {
    if (!hasEepromBeenInitialized) {
      hasEepromBeenInitialized = EEPROM.begin(SIZE);    // Define EEPROM SIZE - Refactor
    }
    return hasEepromBeenInitialized;
  }

  bool ConfigManager::load_configuration(void) {
    if (!initialize_eeprom()) return false;

    // Read identifier
    if (read_identifier() != IDENTIFIER) return false;

    // Read config size
    int offset = sizeof(IDENTIFIER);
    int configSize = (EEPROM.read(offset) << 8) + EEPROM.read(offset+1);

    // Read config
    char buffer[SIZE];
    for (int i = 0; i < configSize; i++) {
      buffer[i] = EEPROM.read(offset+i);
    }

    return ConfigSerializer::deserialize(buffer, SIZE, &_currentConfig);
  }

  bool ConfigManager::save_configuration(void) {
    if (!initialize_eeprom()) return false;

    // Write identifier so we know a config is present in EEPROM
    write_identifier();

    char buffer[SIZE];
    size_t length = ConfigSerializer::serialize(buffer, SIZE, &_currentConfig);

    if (length < 0) return false;

    int offset = sizeof(IDENTIFIER);
    for (int i = 0; i < length; i++) {
      EEPROM.write(offset+i, buffer[i]);
    }
  
    return EEPROM.commit();
  }

  void ConfigManager::factory_default(void) {
    _currentConfig.wifi_ssid(WIFI_DEFAULT_SSID);
    _currentConfig.wifi_password(WIFI_DEFAULT_PASSWORD);
    _currentConfig.mqtt_broker(MQTT_DEFAULT_BROKER);
    _currentConfig.mqtt_port(MQTT_DEFAULT_PORT);
    _currentConfig.mqtt_topic(MQTT_DEFAULT_TOPIC);
    _currentConfig.static_ip(NETWORK_DEFAULT_IP);
    _currentConfig.default_gateway(NETWORK_DEFAULT_GATEWAY);
    _currentConfig.subnet_mask(NETWORK_DEFAULT_SUBNET);
    _currentConfig.read_freq(METER_DEFAULT_FREQ);
  }

  Configuration * ConfigManager::current_config(void) {
    return &_currentConfig;
  }

  void ConfigManager::write_identifier(void) {
    EEPROM.write(0, ((IDENTIFIER >> 8) & 0xFF));
    EEPROM.write(1, (IDENTIFIER & 0xFF));
  }

  uint16_t ConfigManager::read_identifier(void) {
    return (((uint16_t)EEPROM.read(0)) << 8) + EEPROM.read(1);
  }

};