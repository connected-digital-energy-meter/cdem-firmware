/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * Firmware for Feather Huzzah32 to read the P1 port of a Fluvius Smart Meter
 *
*/

// Include general libraries
#include <cstring>
#include <WiFi.h>

// Include custom libraries
#include "hardware.h"
#include "src/rgb/rgb_led.h"
#include "src/config/config_serializer.h"
#include "src/config/config_manager.h"
#include "src/boot/boot_manager.h"
#include "src/smart_meter/smart_digital_meter.h"

// Using the namespace SmartMeter
using namespace SmartMeter;

// Setup the RGB led's on the PCB
RgbLed commLed(COMM_LED_R, COMM_LED_G, COMM_LED_B, 1);

// Predefine some colors
Color NoWifiColor(Color::RED().dim(20));
Color NoMqttColor(Color::ORANGE().dim(20));
Color ComOkColor(Color::GREEN().dim(20));

// Initiate variable of Digitalmeter and configurationmanager.
ConfigManager configManager;

SmartDigitalMeter smartMeter;
TimerHandle_t wifiReconnectTimer;

// Connect to WiFi
void connectToWifi() {
  Serial.println("Connecting to WiFi ...");
  WiFi.begin(
    configManager.current_config()->wifi_ssid().c_str(),
    configManager.current_config()->wifi_password().c_str()
  );
}

// Handle WiFi events
void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      commLed.color(NoMqttColor);
      smartMeter.start(configManager.current_config());
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStart(wifiReconnectTimer, 0);
      commLed.color(NoWifiColor);
      smartMeter.stop();
      break;
  }
}

void setup() {
  // Set the baudrate for both serial connections
  SerialDebug.begin(SERIAL_DEBUG_BAUDRATE);
  SerialDebug.println("Starting Feather Fluvius Meter Reader firmware ...");

  // Initiate the led's on the pcb
  commLed.on();
  commLed.color(NoWifiColor);

  // Loading the EEPROM or factory default configuration settings 
  SerialDebug.println("Loading configuration ...");
  if (configManager.load_configuration()) {
    SerialDebug.println("Loaded existing configuration");
  } else {
    SerialDebug.println("No existing configuration could be loaded");
    SerialDebug.println("Saving factory default configuration");
    configManager.factory_default();
    if (configManager.save_configuration()) {
      SerialDebug.println("Successfully saved configuration");
    } else {
      SerialDebug.println("Something went wrong. Could not save configuration");
    }
  }

  SerialDebug.println("Current Configuration");
  SerialDebug.println(configManager.current_config()->to_string());

  delay(5000);
  SerialDebug.println("Continuing boot procedure ...");

  BootManager bootManager(*configManager.current_config(), &SerialDebug);
  bootManager.Enable_Bootmenu();

  // Setup timers for WiFi and MQTT
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  // Setup eventhandler WiFi
  WiFi.onEvent(WiFiEvent);

  // Make Wifi Connection
  connectToWifi();

  SerialDebug.println("Boot finished");
}

void loop() {
  smartMeter.process();
}
