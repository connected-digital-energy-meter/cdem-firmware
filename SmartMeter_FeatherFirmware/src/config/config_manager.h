/*
 * VIVES college Research
 *
 * Author Ronny Mees & Nico De Witte
 *
 * Class to handle the configuration settings
 *
*/
#pragma once

#include "configuration.h"

namespace SmartMeter {

  class ConfigManager {

    public:
      bool load_configuration(void);
      bool save_configuration(void);
      void factory_default(void);

    public:
      void current_config(Configuration config);
      Configuration current_config(void);

    private:
      bool initialize_eeprom(void);

    private:
      void write_identifier(void);
      uint16_t read_identifier(void);

    private:
      Configuration _currentConfig;
      bool hasEepromBeenInitialized = false;

      static const size_t SIZE = 128;
      static const uint16_t IDENTIFIER = 0xDECB;
  };

};