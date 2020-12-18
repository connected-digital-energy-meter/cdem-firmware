#include "boot_manager.h"
#include "../helpers/serial_helper.h"

namespace SmartMeter {

  //Constructor
  BootManager::BootManager(Configuration current_config, HardwareSerial * userSerial){
    this->current_config=current_config;
    this->userSerial = userSerial;
  }

  void BootManager::Enable_Bootmenu(void) {
    showboot_config = true;
    while (showboot_config) {
      Menu_Action();
    }
  }

  // save settings from EEPROM ??
  void BootManager::Save_Settings(void){
  
  }

  // Get menu selection
  int BootManager::Menu_Selection(void) {
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

  void BootManager::Menu_Action(void) {
    switch (Menu_Selection()) {
      case 1:
        current_config.wifi_ssid(Request_Input("Wifi SSID", current_config.wifi_ssid()));
        break;

      case 2:
        current_config.wifi_password(Request_Input("Wifi Password", current_config.wifi_password()));
        break;

      case 3:
        current_config.static_ip(Request_Input("Static IP", current_config.static_ip()));
        break;

      case 4:
        current_config.subnet_mask(Request_Input("Subnet Mask", current_config.subnet_mask()));
        break;

      case 5:
        current_config.default_gateway(Request_Input("Default Gateway", current_config.default_gateway()));
        break;

      case 6:
        current_config.mqtt_broker(Request_Input("Broker IP", current_config.mqtt_broker()));
        break;

      case 7: {
        int port = atoi(Request_Input("Broker Port", String(current_config.mqtt_port())).c_str());
        current_config.mqtt_port(port);
        break;
      }

      case 8:
        current_config.mqtt_topic(Request_Input("Broker Base topic", current_config.mqtt_topic()));
        break;

      case 9: {
        long freq = atol(Request_Input("Reading frequency", String(current_config.read_freq())).c_str());
        current_config.read_freq(freq);
        break;
      }

      case 10:
        Save_Settings();
        break;            

      case 11:
        showboot_config = false;
        break;
    }
  }

  String BootManager::Request_Input(String info, String current_value) {
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