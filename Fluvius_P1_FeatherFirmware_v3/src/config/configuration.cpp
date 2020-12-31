#include "configuration.h"
#include "factory_default_config.h"

namespace SmartMeter {

  Configuration::Configuration() {
    wifi_ssid(WIFI_DEFAULT_SSID);
    wifi_password(WIFI_DEFAULT_PASSWORD);
    mqtt_broker(MQTT_DEFAULT_BROKER);
    mqtt_port(MQTT_DEFAULT_PORT);
    mqtt_topic(MQTT_DEFAULT_TOPIC);
    static_ip(NETWORK_DEFAULT_IP);
    default_gateway(NETWORK_DEFAULT_GATEWAY);
    subnet_mask(NETWORK_DEFAULT_SUBNET);
    read_period(METER_DEFAULT_READ_PERIOD_SECONDS);
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

  void Configuration::mqtt_topic(String topic) {
    _mqtt_topic = topic;
  }

  String Configuration::mqtt_topic(void) {
    return _mqtt_topic;
  }
      
  void Configuration::static_ip(String ip) {
    _static_ip = ip;
  }
      
  String Configuration::static_ip(void) {
    return _static_ip;
  }
      
  void Configuration::subnet_mask(String mask) {
    _subnet_mask = mask;
  }

  String Configuration::subnet_mask(void) {
    return _subnet_mask;
  }
      
  void Configuration::default_gateway(String gateway) {
    _default_gateway = gateway;
  }
      
  String Configuration::default_gateway(void) {
    return _default_gateway;
  }
      
  void Configuration::read_period(unsigned int period) {
    _read_period = period;
  }
  
  unsigned int Configuration::read_period(void) {
    return _read_period;
  }

  String Configuration::to_string(void) {
    String output = "";
    output += "WiFi SSID:       " + _wifiSSID + "\n";
    output += "WiFi Password:   " + _wifiPassword + "\n";
    output += "Static IP:       " + _static_ip + "\n";
    output += "Subnet Mask:     " + _subnet_mask + "\n";
    output += "Default Gateway: " + _default_gateway + "\n";
    output += "MQTT Broker:     " + _mqttBroker + "\n";
    output += "MQTT Port:       " + String(_mqttPort) + "\n";
    output += "MQTT Topic:      " + _mqtt_topic + "\n";
    output += "Read period:     " + String(_read_period) + " seconds";
    return output;
  }

  bool Configuration::operator==(const Configuration& rhs) {
    return (
      _wifiSSID == rhs._wifiSSID &&
      _wifiPassword == rhs._wifiPassword &&
      _mqttBroker == rhs._mqttBroker &&
      _mqttPort == rhs._mqttPort &&
      _mqtt_topic == rhs._mqtt_topic &&
      _static_ip == rhs._static_ip &&
      _subnet_mask == rhs._subnet_mask &&
      _default_gateway == rhs._default_gateway &&
      _read_period == rhs._read_period
    );
  }

  bool Configuration::operator!=(const Configuration& rhs) {
    return !(*this == rhs);
  }

};