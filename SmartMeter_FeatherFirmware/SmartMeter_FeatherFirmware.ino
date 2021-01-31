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

void connect_to_wifi() {
  deviceStatus.no_communication();

  SerialDebug.println("Connecting to WiFi ...");
  if (!configuration.use_dhcp()) {
    SerialDebug.println("Using static IP ...");
    if (!WiFi.config(
      IPParser::parse_ipv4(configuration.static_ip()),
      IPParser::parse_ipv4(configuration.default_gateway()),
      IPParser::parse_ipv4(configuration.subnet_mask())
      // IPParser::parse_ipv4("8.8.8.8"),  // Google DNS
    )) {
      SerialDebug.println("Failed to configure WiFi. Please check your configuration.");
    } else {
      SerialDebug.println("Using DHCP ...");
    }
  }

  WiFi.begin(
    configuration.wifi_ssid().c_str(),
    configuration.wifi_password().c_str()
  );
}

void setup() {
  SerialDebug.begin(SERIAL_DEBUG_BAUDRATE);
  SerialDebug.println("Starting Connected Digital Energy Meter firmware ...");
  deviceStatus.booting();
  delay(5000);    // Give some time to open serial terminal

  BootManager bootmanager(&deviceStatus);
  configuration = bootmanager.boot();

  deviceStatus.booting();

  SerialDebug.println("Current Configuration");
  SerialDebug.println(configuration.to_string());

  delay(5000);
  SerialDebug.println("Boot procedure finished");
  deviceStatus.done_booting();

  // Setup WiFi (no need for reconnect Timer. WiFi lib has internal reconnect mechanism)
  // The rest is handled by the smart meter
  connect_to_wifi();

  // Start Smart Meter
  SerialDebug.println("Starting smart meter ... ");
  smartMeter.start(&configuration);
}

void loop() {
  smartMeter.process();
}
