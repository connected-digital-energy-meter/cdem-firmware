#pragma once

#include "../rgb/rgb_led.h"

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

    public:   // Communication
      void no_communication(void);
      void wifi_ok(void);
      void wifi_error(void);

      void mqtt_ok(void);
      void mqtt_error(void);

    public:
      void meter_awaiting(void);
      void meter_data_ready(void);
      void meter_error(void);

    private:
      void set_communication_led(void);

    private:
      RgbLed dataLed;
      RgbLed commLed;

      bool wifiOk = false;
      bool mqttOk = false;

      const static unsigned int LED_BRIGHTNESS = 25;

  };

};