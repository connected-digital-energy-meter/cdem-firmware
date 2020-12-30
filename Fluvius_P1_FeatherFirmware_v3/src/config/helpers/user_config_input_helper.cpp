#include "user_config_input_helper.h"
#include "../../validation/number_validator.h"
#include "../../helpers/serial_helper.h"

namespace SmartMeter {

  UserConfigInputHelper::UserConfigInputHelper(HardwareSerial * userSerial) {
    this->userSerial = userSerial;
  }

  void UserConfigInputHelper::configure_wifi_ssid(Configuration * config) {
    config->wifi_ssid(request_input("Wifi SSID", config->wifi_ssid()));
  }

  void UserConfigInputHelper::configure_wifi_password(Configuration * config) {
    config->wifi_password(request_input("Wifi Password", config->wifi_password()));
  }

  void UserConfigInputHelper::configure_static_ip(Configuration * config) {
    config->static_ip(request_input("Network Static IP", config->static_ip()));
  }

  void UserConfigInputHelper::configure_subnet_mask(Configuration * config) {
    config->subnet_mask(request_input("Network Subnet Mask", config->subnet_mask()));
  }

  void UserConfigInputHelper::configure_default_gateway(Configuration * config) {
    config->default_gateway(request_input("Network Default Gateway", config->default_gateway()));
  }

  void UserConfigInputHelper::configure_mqtt_broker(Configuration * config) {
    config->mqtt_broker(request_input("MQTT Broker IP", config->mqtt_broker()));
  }

  void UserConfigInputHelper::configure_mqtt_broker_port(Configuration * config) {
    int port = atoi(request_input("MQTT Broker Port", String(config->mqtt_port())).c_str());
    config->mqtt_port(port);
  }

  void UserConfigInputHelper::configure_mqtt_topic(Configuration * config) {
    config->mqtt_topic(request_input("MQTT Broker Topic", config->mqtt_topic()));
  }

  void UserConfigInputHelper::configure_device_read_period(Configuration * config) {
    NumberValidator validator(1, 3600);
    unsigned int period = atoi(request_input( "Meter Read Period (seconds)",
                                              String(config->read_period()),
                                              &validator
                                            ).c_str());
    config->read_period(period);
  }

  String UserConfigInputHelper::request_input(String key, String value, Validator * validator) {
    bool valid = false;
    String input = "";
    do {
      userSerial->println("");
      userSerial->println("Current " + key + " is " + value);
      userSerial->println("Provide new value for " + key + " or just enter to keep current value.");
      userSerial->println(key + ": ");
      input = SerialHelper::read_line(userSerial);
      userSerial->println("");

      if (input != "" && validator) {
        if (!(valid = validator->is_valid(input))) {
          userSerial->println("Invalid input: " + validator->validation_error());
        }
      } else {
        valid = true;
      }
    } while (!valid);

    return (input == "") ? value : input;
  }
};