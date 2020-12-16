#include "boot_config.h"

namespace SmartMeter {
    //Constructor
    BootManager::BootManager(Configuration current_config){
        this->current_config=current_config;
    }

    void BootManager::Enable_Bootmenu(){
        showboot_config=true;
        while (showboot_config)
        {
            Menu_Action();
        }
    }

    // save settings from EEPROM ??
    void BootManager::Save_Settings(){
    
    }

    // Get menu selection
    int BootManager::Menu_Selection(){
        // Show boot menu
        SerialDebug.println("Config Menu");
        SerialDebug.println("-----------");
        SerialDebug.println("");
        SerialDebug.println("1. Wifi - ssid");
        SerialDebug.println("2. Wifi - password");
        SerialDebug.println("3. Network - Static Ip");
        SerialDebug.println("4. Network - Subnet Mask");
        SerialDebug.println("5. Network - Default Gateway");
        SerialDebug.println("6. Mqtt - Broker Ip");
        SerialDebug.println("7. Mqtt - Broker Port");
        SerialDebug.println("8. Mqtt - Broker base topic");
        SerialDebug.println("9. Reading frequency");
        SerialDebug.println("10. Save configuration");
        SerialDebug.println("11. Exit configuration");
        
        // wait until selection is made
        while (SerialDebug.available() == 0) {}

        // read and return selection
        char menuselection = SerialDebug.read();
        return atoi(menuselection);
    }

    void BootManager::Menu_Action(){
        switch (Menu_Selection())
        {
            case 1:
                current_config.wifi_ssid(Request_Input("Wifi SSID",current_config.wifi_ssid()));
                break;

            case 2:
                current_config.wifi_pass(Request_Input("Wifi Password",current_config.wifi_pass()));
                break;

            case 3:
                current_config.static_ip(Request_Input("Static IP",current_config.static_ip()));
                break;
    
            case 4:
                current_config.subnet_mask(Request_Input("Subnet Mask",current_config.subnet_mask()));
                break;

            case 5:
                current_config.default_gateway(Request_Input("Default Gateway",current_config.default_gateway()));
                break;

            case 6:
                current_config.mqtt_broker(Request_Input("Broker IP",current_config.mqtt_broker()));
                break;

            case 7:
                current_config.mqtt_port(Request_Input("Broker Port",current_config.mqtt_port()));
                break;

            case 8:
                current_config.mqtt_topic(Request_Input("Broker Base topic",current_config.mqtt_topic()));
                break;

            case 9:
                current_config.read_freq(Request_Input("Reading frequency",current_config.read_freq()));
                break;

            case 10:
                Save_Settings();
                break;            

            case 11:
                show_bootconfig=false;
                break;
        }
    }

    String BootManager::Request_Input(String info, String current_value){
        // Show current value
        SerialDebug.println("-----------");
        SerialDebug.println("");
        SerialDebug.print("Current "+info+" = ");
        SerialDebug.println(current_value);
        SerialDebug.println("Please enter your "+info);

        SerialDebug.flush();
    
        // wait until new input
        while (SerialDebug.available() == 0) {}

        // Read and return the input
        return SerialDebug.readStringUntil('\n');
    }

   
}