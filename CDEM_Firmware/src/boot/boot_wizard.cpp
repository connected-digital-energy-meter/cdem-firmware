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
        userSerial->println("#########################################################");
        userSerial->println("# Connected Digital Energy Meter - Configuration Wizard #");
        userSerial->println("#########################################################");
        userSerial->println("");
        userSerial->println("------------------------------------------");
        userSerial->println("Let us first configure your WiFi settings:");
        userSerial->println("------------------------------------------");
        userConfigInputHelper.configure_wifi_ssid(&newConfig);
        userSerial->println("");
        userConfigInputHelper.configure_wifi_password(&newConfig);
        userSerial->println("");
        userSerial->println("-------------------------------------------");
        userSerial->println("Next configure your local network settings:");
        userSerial->println("-------------------------------------------");
        userConfigInputHelper.configure_use_dhcp(&newConfig);
        if (!newConfig.use_dhcp()) {
          userSerial->println("");
          userConfigInputHelper.configure_static_ip(&newConfig);
          userSerial->println("");
          userConfigInputHelper.configure_subnet_mask(&newConfig);
          userSerial->println("");
          userConfigInputHelper.configure_default_gateway(&newConfig);
        }
        userSerial->println("");
        userSerial->println("----------------------------------------------------------------------------");
        userSerial->println("This device will publish it's data to an MQTT broker.");
        userSerial->println("In order to do so, it will need the IP address and port of your MQTT broker.");
        userSerial->println("It also needs a base topic where the data can be published on.");
        userSerial->println("----------------------------------------------------------------------------");
        userConfigInputHelper.configure_mqtt_broker(&newConfig);
        userSerial->println("");
        userConfigInputHelper.configure_mqtt_broker_port(&newConfig);
        userSerial->println("");
        userConfigInputHelper.configure_mqtt_topic(&newConfig);
        userSerial->println("");
        userSerial->println("----------------------------------------------------------");
        userSerial->println("Finally, we need to configure the time between data reads.");
        userSerial->println("----------------------------------------------------------");
        userConfigInputHelper.configure_device_read_period(&newConfig);
        userSerial->println("");
        userSerial->println("Thank you for using the configuration wizard ...");
        userSerial->println("");
        userSerial->println("");

        return newConfig;
    }

}