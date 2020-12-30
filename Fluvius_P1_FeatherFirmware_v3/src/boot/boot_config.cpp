#include "boot_config.h"
#include "../helpers/serial_helper.h"

// TODO - Add option for using DHCP (boolean)
// TODO - Allow wizard to be started
// TODO - Reset to Defaults option

namespace SmartMeter {

  // Constructor
  BootConfig::BootConfig(Configuration currentConfig, HardwareSerial * userSerial)
    : userConfigInputHelper(userSerial) {

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

        case 4:
          userSerial->println("TODO - Use Configuration Wizard - NOT IMPLEMENTED YET!!!");
          break;

        case 5:
          restore_factory_defaults();
          break;
      }
    } while (menuSelection < 6);

    if (menuSelection == 6) return newConfig;
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
      userSerial->println("4. TODO - Use Configuration Wizard");
      userSerial->println("5. Restore Factory Defaults");
      userSerial->println("6. Save & Continue Boot");
      userSerial->println("7. Discard & Continue Boot");
      userSerial->println("");
      userSerial->print("Please pick an option [1-6]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 7);

    return choice;
  }

  void BootConfig::configure_network(void) {
    int selection = 0;
    do {
      selection = request_network_menu_selection();
      switch (selection) {
        case 1:
          userConfigInputHelper.configure_wifi_ssid(&newConfig);
          break;
        case 2:
          userConfigInputHelper.configure_wifi_password(&newConfig);
          break;
        case 3:
          userConfigInputHelper.configure_static_ip(&newConfig);
          break;
        case 4:
          userConfigInputHelper.configure_subnet_mask(&newConfig);
          break;
        case 5:
          userConfigInputHelper.configure_default_gateway(&newConfig);
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
      userSerial->println("");
      userSerial->print("Please pick an option [1-6]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 6);

    return choice;
  }

  void BootConfig::configure_mqtt(void) {
    int selection = 0;
    do {
      selection = request_mqtt_menu_selection();
      switch (selection) {
        case 1:
          userConfigInputHelper.configure_mqtt_broker(&newConfig);
          break;
        case 2:
          userConfigInputHelper.configure_mqtt_broker_port(&newConfig);
          break;
        case 3:
          userConfigInputHelper.configure_mqtt_topic(&newConfig);
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
      userSerial->println("");
      userSerial->print("Please pick an option [1-4]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 4);

    return choice;
  }

  void BootConfig::configure_meter(void) {
    int selection = 0;
    do {
      selection = request_meter_menu_selection();
      switch (selection) {
        case 1:
          userConfigInputHelper.configure_device_read_period(&newConfig);
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
      userSerial->println("1. Change Read Period [" + String(newConfig.read_period()) + " seconds]");
      userSerial->println("2. Return");
      userSerial->println("");
      userSerial->print("Please pick an option [1-2]: ");
      choice = atoi(SerialHelper::read_line(userSerial).c_str());
      userSerial->println("");
    } while (choice < 1 && choice > 2);

    return choice;
  }

  void BootConfig::restore_factory_defaults(void) {
    String input = "";
    do {
      userSerial->println("");
      userSerial->println("Are you sure you wish to reset the configuration to factory defaults? [y/N]");
      input = SerialHelper::read_line(userSerial);
      input.toLowerCase();
      userSerial->println("");
    } while (input != "y" && input != "n" && input != "");

    if (input == "y") {
      newConfig = Configuration();
    }
  }

}