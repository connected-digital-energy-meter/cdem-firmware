#include "configuration.h"

namespace SmartMeter {

  Configuration::Configuration() {
    _wifiSSID = "SSID";
    _wifiPassword = "PASSWORD";
    _mqttBroker = "BROKER";
    _mqttPort = 1883;
  }

  void Configuration::wifi_ssid(String ssid) {
    _wifiSSID = ssid;
  }

  String Configuration::wifi_ssid(void) {
    return _wifiSSID;
  }

  void Configuration::wifi_password(String password) {
    _wifiPassword = password;
  }

  String Configuration::wifi_password(void) {
    return _wifiPassword;
  }

  void Configuration::mqtt_broker(String broker) {
    _mqttBroker = broker;
  }

  String Configuration::mqtt_broker(void) {
    return _mqttBroker;
  }

  void Configuration::mqtt_port(int port) {
    _mqttPort = port;
  }

  int Configuration::mqtt_port(void) {
    return _mqttPort;
  }

};