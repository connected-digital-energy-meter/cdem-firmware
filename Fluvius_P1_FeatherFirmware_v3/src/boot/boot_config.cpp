#include "boot_config.h"
#include "../helpers/serial_helper.h"

// TODO - Add option for using DHCP (boolean)
// TODO - Allow wizard to be started
// TODO - Reset to Defaults option

namespace SmartMeter {

  // Constructor
  BootConfig::BootConfig(Configuration currentConfig, HardwareSerial * userSerial) {
    this->originalConfig = currentConfig;
    reset_new_config();
    this->userSerial = userSerial;
  }

  void BootConfig::reset_new_config(void) {
    newConfig = originalConfig;
  }

  Configuration BootConfig::enable_boot_menu(void) {
    int menuSelection = 0;
    do {
      menuSelection = request_menu_selection();
    
      switch (menuSelection) {
        case 1:
          configure_network();
          break;

        case 2:
          configure_mqtt();
          break;

        case 3:
          configure_meter();
          break;
      }
    } while (menuSelection < 4);

    if (menuSelection == 4) return newConfig;
    else return originalConfig;
  }

  int BootConfig::request_menu_selection(void) {
    int choice = 0;

    do {
      userSerial->println("");
      userSerial->println("###########################");
      userSerial->println("# Smart Meter Boot Config #");
      userSerial->println("###########################");
      userSerial->println("");
      userSerial->println("Current configuration:");
      userSerial->println("----------------------");
      userSerial->println(newConfig.to_string());
      userSerial->println("###########################");
      userSerial->println("");
      userSerial->println("Configuration Options:");
      userSerial->println("----------------------");
      userSerial->println("1. Change Network/WiFi settings");
      userSerial->println("2. Change MQTT settings");
      userSerial->println("3. Change Meter settings");
      userSerial->println("4. Save & Continue Boot");
      userSerial->println("5. Discard & Continue Boot");
      userSerial->print("Please pick an option [1-6]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 5);

    return choice;
  }

  void BootConfig::configure_network(void) {
    int selection = 0;
    do {
      selection = request_network_menu_selection();
      switch (selection) {
        case 1:
          configure_wifi_ssid();
          break;
        case 2:
          configure_wifi_password();
          break;
        case 3:
          configure_static_ip();
          break;
        case 4:
          configure_subnet_mask();
          break;
        case 5:
          configure_default_gateway();
          break;
      }
    } while (selection != 6);
  }

  int BootConfig::request_network_menu_selection(void) {
    int choice = 0;

    do {
      userSerial->println("");
      userSerial->println("Network Configuration");
      userSerial->println("------------------");
      userSerial->println("1. Change WiFi SSID [" + newConfig.wifi_ssid() + "]");
      userSerial->println("2. Change WiFi Password [" + newConfig.wifi_password() + "]");
      userSerial->println("3. Change Static IP [" + newConfig.static_ip() + "]");
      userSerial->println("4. Change Subnet Mask [" + newConfig.subnet_mask() + "]");
      userSerial->println("5. Change Default Gateway [" + newConfig.default_gateway() + "]");
      userSerial->println("6. Return");
      userSerial->print("Please pick an option [1-6]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 6);

    return choice;
  }

  void BootConfig::configure_wifi_ssid(void) {
    newConfig.wifi_ssid(request_input("Wifi SSID", newConfig.wifi_ssid()));
  }

  void BootConfig::configure_wifi_password(void) {
    newConfig.wifi_password(request_input("Wifi Password", newConfig.wifi_password()));
  }

  void BootConfig::configure_static_ip(void) {
    newConfig.static_ip(request_input("Network Static IP", newConfig.static_ip()));
  }

  void BootConfig::configure_subnet_mask(void) {
    newConfig.subnet_mask(request_input("Network Subnet Mask", newConfig.subnet_mask()));
  }

  void BootConfig::configure_default_gateway(void) {
    newConfig.default_gateway(request_input("Network Default Gateway", newConfig.default_gateway()));
  }

  void BootConfig::configure_mqtt(void) {
    int selection = 0;
    do {
      selection = request_mqtt_menu_selection();
      switch (selection) {
        case 1:
          configure_mqtt_broker();
          break;
        case 2:
          configure_mqtt_broker_port();
          break;
        case 3:
          configure_mqtt_topic();
          break;
      }
    } while (selection != 4);
  }

  int BootConfig::request_mqtt_menu_selection(void) {
    int choice = 0;

    do {
      userSerial->println("");
      userSerial->println("MQTT Configuration");
      userSerial->println("------------------");
      userSerial->println("1. Change MQTT Broker [" + newConfig.mqtt_broker() + "]");
      userSerial->println("2. Change MQTT Port [" + String(newConfig.mqtt_port()) + "]");
      userSerial->println("3. Change MQTT Base Topic [" + newConfig.mqtt_topic() + "]");
      userSerial->println("4. Return");
      userSerial->print("Please pick an option [1-4]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 4);

    return choice;
  }

  void BootConfig::configure_mqtt_broker(void) {
    newConfig.mqtt_broker(request_input("MQTT Broker IP", newConfig.mqtt_broker()));
  }

  void BootConfig::configure_mqtt_broker_port(void) {
    int port = atoi(request_input("MQTT Broker Port", String(newConfig.mqtt_port())).c_str());
    newConfig.mqtt_port(port);
  }

  void BootConfig::configure_mqtt_topic(void) {
    newConfig.mqtt_topic(request_input("MQTT Broker Topic", newConfig.mqtt_topic()));
  }

  void BootConfig::configure_meter(void) {
    int selection = 0;
    do {
      selection = request_meter_menu_selection();
      switch (selection) {
        case 1:
          configure_device_read_period();
          break;
      }
    } while (selection != 2);
  }

  int BootConfig::request_meter_menu_selection(void) {
    int choice = 0;

    do {
      userSerial->println("");
      userSerial->println("Meter Configuration");
      userSerial->println("------------------");
      userSerial->println("1. Change Read Period [" + String(newConfig.read_freq()) + "]");
      userSerial->println("2. Return");
      userSerial->print("Please pick an option [1-2]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 2);

    return choice;
  }

  void BootConfig::configure_device_read_period(void) {
    long period = atol(request_input("Meter Read Period", String(newConfig.read_freq())).c_str());
    newConfig.read_freq(period);
  }

  String BootConfig::request_input(String key, String value) {
    userSerial->println("");
    userSerial->println("Current " + key + " is " + value);
    userSerial->println("Provide new value for " + key + " or just enter to keep current value");
    userSerial->println(key + ": ");
    String input = SerialHelper::read_line(userSerial);
    userSerial->println("");

    return (input == "") ? value : input;
  }
   
}