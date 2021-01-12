#include "user_config_input_helper.h"
#include "../../validation/number_validator.h"
#include "../../validation/ip_validator.h"
#include "../../validation/choice_validator.h"
#include "../../validation/mqtt_topic_validator.h"
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

  void UserConfigInputHelper::configure_use_dhcp(Configuration * config) {
    std::vector<String> options;
    options.push_back("yes");
    options.push_back("no");

    ChoiceValidator validator(options, false);
    String useDhcp = request_input("Use DHCP?",
                                    String(config->use_dhcp() ? "yes" : "no"),
                                    &validator,
                                    "[yes|no]"
                                    );
    useDhcp.toLowerCase();
    config->use_dhcp(useDhcp == "yes");
  }

  void UserConfigInputHelper::configure_static_ip(Configuration * config) {
    IpValidator validator;
    config->static_ip(request_input("Network Static IP",
                                    config->static_ip(),
                                    &validator,
                                    "[0-255].[0-255].[0-255].[0-255] - Example 192.168.1.20"
                                    ));
  }

  void UserConfigInputHelper::configure_subnet_mask(Configuration * config) {
    IpValidator validator;
    config->subnet_mask(request_input("Network Subnet Mask",
                                    config->subnet_mask(),
                                    &validator,
                                    "[0-255].[0-255].[0-255].[0-255] - Example 255.255.0.0"
                                    ));
  }

  void UserConfigInputHelper::configure_default_gateway(Configuration * config) {
    IpValidator validator;
    config->default_gateway(request_input("Network Default Gateway",
                                    config->default_gateway(),
                                    &validator,
                                    "[0-255].[0-255].[0-255].[0-255] - Example 192.168.0.1"
                                    ));
  }

  void UserConfigInputHelper::configure_mqtt_broker(Configuration * config) {
    IpValidator validator;
    config->mqtt_broker(request_input("MQTT Broker IP",
                                    config->mqtt_broker(),
                                    &validator,
                                    "[0-255].[0-255].[0-255].[0-255] - Example 192.168.1.1"
                                    ));
  }

  void UserConfigInputHelper::configure_mqtt_broker_port(Configuration * config) {
    NumberValidator validator(1, 65353);
    unsigned int port = atoi(request_input( "MQTT Broker Port",
                                              String(config->mqtt_port()),
                                              &validator,
                                              "[1-65353] - Example 1883"
                                            ).c_str());
    config->mqtt_port(port);
  }

  void UserConfigInputHelper::configure_mqtt_topic(Configuration * config) {
    MqttTopicValidator validator;
    config->mqtt_topic(request_input("MQTT Broker Topic",
                                    config->mqtt_topic(),
                                    &validator,
                                    "level/level/level - myhome/metering/smartmeter"
                                    ));
  }

  void UserConfigInputHelper::configure_device_read_period(Configuration * config) {
    NumberValidator validator(1, 3600);
    unsigned int period = atoi(request_input( "Meter Read Period (seconds)",
                                              String(config->read_period()),
                                              &validator,
                                              "[1-3600] - Example 10"
                                            ).c_str());
    config->read_period(period);
  }

  String UserConfigInputHelper::request_input(String key, String value, Validator * validator, String expectedFormat) {
    bool valid = false;
    String input = "";
    do {
      userSerial->println("");
      userSerial->println("Provide new value for " + key + " or just enter to keep current value.");
      if (expectedFormat != "") {
        userSerial->println("Expected input format: " + expectedFormat);
      }
      userSerial->println("");
      userSerial->println(key + " [" + value + "]: ");
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