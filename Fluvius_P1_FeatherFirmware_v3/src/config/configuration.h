#pragma once

#include <Arduino.h>

namespace SmartMeter {

  class Configuration {

    public:
      Configuration();

    public:
      void wifi_ssid(String ssid);
      String wifi_ssid(void);

      void wifi_password(String password);
      String wifi_password(void);

      void mqtt_broker(String broker);
      String mqtt_broker(void);

      void mqtt_port(int port);
      int mqtt_port(void);

    private:
      String _wifiSSID;
      String _wifiPassword;
      String _mqttBroker;
      int _mqttPort;
  };

};