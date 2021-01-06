/*
 * VIVES college Research
 *
 * Author Ronny Mees & Nico De Witte
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
#include "src/boot/boot_manager.h"
#include "src/smart_meter/smart_digital_meter.h"
#include "src/helpers/ip_parser.h"

// Using the namespace SmartMeter
using namespace SmartMeter;

// Setup the RGB led's on the PCB
RgbLed commLed(COMM_LED_R, COMM_LED_G, COMM_LED_B, 1, true);

// Predefine some colors
Color NoWifiColor(Color::RED().dim(20));
Color NoMqttColor(Color::ORANGE().dim(20));
Color ComOkColor(Color::GREEN().dim(20));

// Initiate variable of Digitalmeter and configurationmanager.
Configuration configuration;
SmartDigitalMeter smartMeter;
TimerHandle_t wifiReconnectTimer;

// Connect to WiFi
void connectToWifi() {
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
      commLed.color(NoMqttColor);
      smartMeter.start(&configuration);
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      SerialDebug.println("WiFi lost connection");
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
  delay(5000);    // Give some time to open serial terminal

  // Initiate the led's on the pcb
  commLed.on();
  commLed.color(NoWifiColor);

  BootManager bootmanager;
  configuration = bootmanager.boot();

  SerialDebug.println("Current Configuration");
  SerialDebug.println(configuration.to_string());

  delay(5000);
  SerialDebug.println("Continuing boot procedure ...");

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
