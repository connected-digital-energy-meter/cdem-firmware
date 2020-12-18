#include "configuration.h"

namespace SmartMeter {

  Configuration::Configuration() {
    _wifiSSID = "SSID";
    _wifiPassword = "PASSWORD";
    _mqttBroker = "BROKER";
    _mqttPort = 1883;
    _mqtt_topic = "TOPIC";
    _static_ip = "0.0.0.0";
    _subnet_mask = "0.0.0.0";
    _default_gateway = "0.0.0.0";
    _read_freq = 60000;
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

  void Configuration::mqtt_topic(String topic){
    _mqtt_topic = topic;
  }

  String Configuration::mqtt_topic(void){
    return _mqtt_topic;
  }
      
  void Configuration::static_ip(String ip){
    _static_ip = ip;
  }
      
  String Configuration::static_ip(void){
    return _static_ip;
  }
      
  void Configuration::subnet_mask(int mask){
    _subnet_mask = mask;
  }

  String Configuration::subnet_mask(void){
    return _subnet_mask;
  }
      
  void Configuration::default_gateway(String gateway){
    _default_gateway = gateway;
  }
      
  String Configuration::default_gateway(void){
    return _default_gateway;
  }
      
  void Configuration::read_freq(long freq){
    _read_freq = freq;
  }
  
  long Configuration::read_freq(void){
    return _read_freq
  }

  String Configuration::to_string(void) {
    String output = "";
    output += "SSID: " + _wifiSSID + "\n";
    output += "Password: " + _wifiPassword + "\n";
    output += "Broker: " + _mqttBroker + "\n";
    output += "Port: " + String(_mqttPort);
    output += "Topic: " + _mqtt_topic;
    output += "Static IP: " + _static_ip;
    output += "Subnet Mask: " + _subnet_mask;
    output += "Default Gateway: " + _default_gateway;
    output += "Read frequency: " + String(_read_freq);
    return output;
  }

};