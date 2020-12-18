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
      
      void mqtt_topic(String topic);
      String mqtt_topic(void);
      
      void static_ip(String ip);
      String static_ip(void);
      
      void subnet_mask(String mask);
      String subnet_mask(void);
      
      void default_gateway(String gateway);
      String default_gateway(void);
      
      void read_freq(long freq);
      long read_freq(void);
      
    public:
      String to_string(void);

    private:
      String _wifiSSID;
      String _wifiPassword;
      String _mqttBroker;
      int _mqttPort;
      String _mqtt_topic;
      String _static_ip;
      String _subnet_mask;
      String _default_gateway;
      long _read_freq;
  };

};