#include "configuration.h"

namespace SmartMeter {

  Configuration::Configuration() {
    _wifiSSID = "SSID";
    _wifiPassword = "PASSWORD";
    _mqttBroker = "BROKER";
    _mqttPort = 1883;
    _mqtt_topc = "TOPIC";
    _default_IP = "";
    _subnet_mask = "";
    _default_gateway = "";
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

  void mqtt_topic(String topic){
    _mqtt_topic = topic;
  }

  String mqtt_topic(void){
    return _mqtt_topic;
  }
      
  void static_ip(String ip){
    _static_ip = ip;
  }
      
  String static_ip(void){
    return _static_ip;
  }
      
  void subnet_mask(int mask){
    _subnet_mask = mask;
  }

  String subnet_mask(void){
    return _subnet_mask;
  }
      
  void default_gateway(String gateway){
    _default_gateway = gateway;
  }
      
  String default_gateway(void){
    return _default_gateway;
  }
      
  void read_freq(int freq){
    _read_freq = freq;
  }
  
  int read_freq(void){
    return _read_freq
  }

  String Configuration::to_string(void) {
    String output = "";
    output += "SSID: " + _wifiSSID + "\n";
    output += "Password: " + _wifiPassword + "\n";
    output += "Broker: " + _mqttBroker + "\n";
    output += "Port: " + String(_mqttPort);
    output += "Topic: " + String(_mqtt_topic);
    output += "Static IP: " + String(_static_ip);
    output += "Subnet Mask: " + String(_subnet_mask);
    output += "Default Gateway: " + String(_default_gateway);
    output += "Read frequency: " + String(_read_freq);
    return output;
  }

};