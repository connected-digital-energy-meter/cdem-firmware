#include "boot_wizard.h"
#include "../helpers/serial_helper.h"

namespace SmartMeter {
    BootWizard::BootWizard(Configuration currentConfig, HardwareSerial * userSerial){
        this->originalConfig = currentConfig;
        reset_new_config();
        this->userSerial = userSerial;
    }   

    void BootWizard::reset_new_config(void) {
        newConfig = originalConfig;
    }

    Configuration RunWizard(){
        // Upon first startup we could load the factory defaults to start with.
        userSerial->println("First let us configure your Wifi connection.");
        userSerial->println("-----------");
        userSerial->println("");
        userSerial->println("Your current Wifi SSID is ", newConfig->wifi_ssid());
        userSerial->print("Please enter your new SSID: ");
        newConfig->wifi_ssid(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("Your current Wifi Password is ", newConfig->wifi_password());
        userSerial->print("Please enter your new password: ");
        newConfig->wifi_password(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("Next we configure your Network connection.");
        userSerial->println("-----------");
        userSerial->println("");
        userSerial->println("Your current Static IP is ", newConfig->static_ip());
        userSerial->print("Please enter your new Static IP: ");
        newConfig->static_ip(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("Your current Subnet Mask is ", newConfig->subnet_mask());
        userSerial->print("Please enter your new Subnet Mask: ");
        newConfig->subnet_mask(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("Your current Default Gateway is ", newConfig->default_gateway());
        userSerial->print("Please enter your new Default Gateway: ");
        newConfig->default_gateway(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("This device will post it's data to a MQTT broker.");
        userSerial->println("In order to do this we will need the IP-adress and port of your MQTT broker.");
        userSerial->println("We also need the base topic in your MQTT broker where we should post the data.");
        userSerial->println("-----------");
        userSerial->println("");
        userSerial->println("Your current MQTT broker IP adress is ", newConfig->mqtt_broker());
        userSerial->print("Please enter your new MQTT broker IP adress: ");
        newConfig->mqtt_broker(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("Your current MQTT broker Port is ", newConfig->mqtt_port());
        userSerial->print("Please enter your new MQTT broker Port: ");
        newConfig->mqtt_port(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("Your current MQTT broker Base Topic is ", newConfig->mqtt_topic());
        userSerial->print("Please enter your new MQTT broker Base Topic: ");
        newConfig->mqtt_topic(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate
        userSerial->println("");
        userSerial->println("Finaly we need the frequency you want to upload your data to the MQTT broker.");
        userSerial->println("-----------");
        userSerial->println("");
        userSerial->println("Your current frequency is ", newConfig->read_freq(), " s");   // this is in seconds rest of firmware is in milliseconds, we should change all to seconds.
        userSerial->print("Please enter your frequency (in seconds) : ");
        newConfig->read_freq(SerialHelper::read_line(userSerial).c_str()); // TODO - Validate

        return newConfig;
    }

}