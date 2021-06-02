#pragma once

#include "../rgb/rgb_led.h"
#include "../rgb/effects/effect.h"

namespace SmartMeter {

  class DeviceStatus {

    public:
      DeviceStatus(void);

    public:   // Both LED's
      void clear(void);
      void booting(void);
      void boot_menu(void);
      void config_wizard(void);
      void no_wifi_no_mqtt(void);
      void done_booting(void);

    public:   // Communication
      void no_communication(void);
      void wifi_no_mqtt(void);
      void wifi_and_mqtt_ok(void);

    public:   // Data
      void meter_starting(void);
      void meter_data_ready(void);
      void meter_error(void);

    private:
      void set_communication_led(void);

    private:
      void set_duo_effect(Effects::Effect * commEffect, Effects::Effect * dataEffect);

    private:
      RgbLed dataLed;
      RgbLed commLed;
  };

};