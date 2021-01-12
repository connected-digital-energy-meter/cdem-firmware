/*
 * VIVES college Research
 *
 * Author Ronny Mees & Nico De Witte
 *
 * Firmware for Feather Huzzah32 to read the P1 port of a Fluvius Smart Meter
 *
*/

// Include general libraries
#include <WiFi.h>

// Include custom libraries
#include "hardware.h"
#include "src/boot/boot_manager.h"
#include "src/smart_meter/smart_digital_meter.h"
#include "src/helpers/ip_parser.h"
#include "src/status/device_status.h"

// Using the namespace SmartMeter
using namespace SmartMeter;

// Display device status to user
DeviceStatus deviceStatus;

// Initiate variable of Digitalmeter and configurationmanager.
Configuration configuration;
SmartDigitalMeter smartMeter(&deviceStatus);
TimerHandle_t wifiReconnectTimer;

// Connect to WiFi
void connectToWifi() {
  deviceStatus.no_communication();

  SerialDebug.println("Connecting to WiFi ...");
  if (!configuration.use_dhcp()) {
    if (!WiFi.config(
      IPParser::parse_ipv4(configuration.static_ip()),
      IPParser::parse_ipv4(configuration.default_gateway()),
      IPParser::parse_ipv4(configuration.subnet_mask())
      // IPParser::parse_ipv4("8.8.8.8"),  // Google DNS
    )) {
      SerialDebug.println("Failed to configure WiFi. Please check your configuration.");
    }
  }

  WiFi.begin(
    configuration.wifi_ssid().c_str(),
    configuration.wifi_password().c_str()
  );
}

// Handle WiFi events
void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      SerialDebug.print("WiFi connected with IP Address: ");
      SerialDebug.println(WiFi.localIP());
      deviceStatus.wifi_ok();
      // smartMeter.start(&configuration);
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      SerialDebug.println("WiFi lost connection");
      xTimerStart(wifiReconnectTimer, 0);
      deviceStatus.no_communication();
      // smartMeter.stop();
      break;
    default: break;
  }
}

void setup() {
  // Set the baudrate for both serial connections
  SerialDebug.begin(SERIAL_DEBUG_BAUDRATE);
  SerialDebug.println("Starting Feather Fluvius Meter Reader firmware ...");
  deviceStatus.booting();
  delay(5000);    // Give some time to open serial terminal

  BootManager bootmanager(&deviceStatus);
  configuration = bootmanager.boot();

  deviceStatus.booting();

  SerialDebug.println("Current Configuration");
  SerialDebug.println(configuration.to_string());

  delay(5000);
  SerialDebug.println("Continuing boot procedure ...");

  // Setup timers for WiFi and MQTT
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  deviceStatus.done_booting();

  // Setup eventhandler WiFi
  WiFi.onEvent(WiFiEvent);

  // Make Wifi Connection
  connectToWifi();

  SerialDebug.println("Starting meter ... ");
  smartMeter.start(&configuration);
  SerialDebug.println("Boot finished");
}

void loop() {
  smartMeter.process();
}
