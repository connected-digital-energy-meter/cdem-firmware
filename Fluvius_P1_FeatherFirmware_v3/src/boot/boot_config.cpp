#include "boot_config.h"
#include "../helpers/serial_helper.h"

namespace SmartMeter {

  //Constructor
  BootConfig::BootConfig(Configuration currentConfig, HardwareSerial * userSerial) {
    this->originalConfig = currentConfig;
    reset_new_config();
    this->userSerial = userSerial;
  }

  void BootConfig::reset_new_config(void) {
    newConfig = originalConfig;
  }

  Configuration BootConfig::Enable_Bootmenu(void) {
    showboot_config = true;
    while (showboot_config) {
      Menu_Action();
    }

    return (returnNewConfig ? newConfig : originalConfig);
  }

  // Get menu selection
  int BootConfig::Menu_Selection(void) {
    // Show boot menu
    userSerial->println("Config Menu");
    userSerial->println("-----------");
    userSerial->println("");
    userSerial->println("1. Wifi - ssid");
    userSerial->println("2. Wifi - password");
    userSerial->println("3. Network - Static Ip");
    userSerial->println("4. Network - Subnet Mask");
    userSerial->println("5. Network - Default Gateway");
    userSerial->println("6. Mqtt - Broker Ip");
    userSerial->println("7. Mqtt - Broker Port");
    userSerial->println("8. Mqtt - Broker base topic");
    userSerial->println("9. Reading frequency");
    userSerial->println("10. Save configuration");
    userSerial->println("11. Exit configuration");
    
    userSerial->print("Please pick an option [1-11]: ");
    int choice = atoi(SerialHelper::read_line(userSerial).c_str());   // TODO - Validate
    
    return choice;
  }

  void BootConfig::Menu_Action(void) {
    switch (Menu_Selection()) {
      case 1:
        newConfig.wifi_ssid(Request_Input("Wifi SSID", newConfig.wifi_ssid()));
        break;

      case 2:
        newConfig.wifi_password(Request_Input("Wifi Password", newConfig.wifi_password()));
        break;

      case 3:
        newConfig.static_ip(Request_Input("Static IP", newConfig.static_ip()));
        break;

      case 4:
        newConfig.subnet_mask(Request_Input("Subnet Mask", newConfig.subnet_mask()));
        break;

      case 5:
        newConfig.default_gateway(Request_Input("Default Gateway", newConfig.default_gateway()));
        break;

      case 6:
        newConfig.mqtt_broker(Request_Input("Broker IP", newConfig.mqtt_broker()));
        break;

      case 7: {
        int port = atoi(Request_Input("Broker Port", String(newConfig.mqtt_port())).c_str());
        newConfig.mqtt_port(port);
        break;
      }

      case 8:
        newConfig.mqtt_topic(Request_Input("Broker Base topic", newConfig.mqtt_topic()));
        break;

      case 9: {
        long freq = atol(Request_Input("Reading frequency", String(newConfig.read_freq())).c_str());
        newConfig.read_freq(freq);
        break;
      }

      case 10:
        returnNewConfig = true;
        break;            

      case 11:
        showboot_config = false;
        break;
    }
  }

  String BootConfig::Request_Input(String info, String current_value) {
    // Show current value
    userSerial->println("-----------");
    userSerial->println("");
    userSerial->print("Current "+info+" = ");
    userSerial->println(current_value);
    userSerial->println("Please enter your "+info);

    String input = SerialHelper::read_line(userSerial);
    return input;
  }
   
}