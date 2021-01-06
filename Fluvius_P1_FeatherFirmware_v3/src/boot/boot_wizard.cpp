#include "boot_wizard.h"
#include "../helpers/serial_helper.h"

namespace SmartMeter {
    BootWizard::BootWizard(Configuration currentConfig, HardwareSerial * userSerial)
        : userConfigInputHelper(userSerial) {
            this->originalConfig = currentConfig;
            reset_new_config();
            this->userSerial = userSerial;
    }   

    void BootWizard::reset_new_config(void) {
        newConfig = originalConfig;
    }

    Configuration BootWizard::RunWizard(void) {
        userSerial->println("");
        userSerial->println("#############################");
        userSerial->println("# Smart Meter Config Wizard #");
        userSerial->println("#############################");
        userSerial->println("");
        userSerial->println("First let us configure your Wifi connection.");
        userSerial->println("-----------");
        userConfigInputHelper.configure_wifi_ssid(&newConfig);
        userSerial->println("");
        userConfigInputHelper.configure_wifi_password(&newConfig);
        userSerial->println("");
        userSerial->println("Next we configure your Network connection.");
        userSerial->println("-----------");
        userSerial->println("");
        userSerial->println("");
        userConfigInputHelper.configure_static_ip(&newConfig);
        userSerial->println("");
        userConfigInputHelper.configure_subnet_mask(&newConfig);
        userSerial->println("");
        userConfigInputHelper.configure_default_gateway(&newConfig);
        userSerial->println("");
        userSerial->println("This device will post it's data to a MQTT broker.");
        userSerial->println("In order to do this we will need the IP-address and port of your MQTT broker.");
        userSerial->println("We also need the base topic in your MQTT broker where we should post the data.");
        userSerial->println("-----------");
        userSerial->println("");
        userConfigInputHelper.configure_mqtt_broker(&newConfig);
        userSerial->println("");
        userConfigInputHelper.configure_mqtt_broker_port(&newConfig);
        userSerial->println("");
        userSerial->println("Finaly we need the read period you want to upload your data to the MQTT broker.");
        userSerial->println("-----------");
        userSerial->println("");
        userConfigInputHelper.configure_device_read_period(&newConfig);
        return newConfig;
    }

}