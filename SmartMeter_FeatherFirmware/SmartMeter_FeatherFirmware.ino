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
#include "version.h"
#include "src/boot/boot_manager.h"
#include "src/smart_meter/smart_digital_meter.h"
#include "src/helpers/ip_parser.h"
#include "src/status/device_status.h"
#include "src/logging/logger.h"

// Using the namespace SmartMeter
using namespace SmartMeter;

// Display device status to user
DeviceStatus deviceStatus;

// Initiate variable of Digitalmeter and configurationmanager.
Configuration configuration;
SmartDigitalMeter smartMeter(&deviceStatus);

void connect_to_wifi() {
  DoLog.info("Connecting to WiFi ...", "wifi");
  if (!configuration.use_dhcp()) {
    DoLog.verbose("Using static IP ...", "wifi");
    if (!WiFi.config(
      IPParser::parse_ipv4(configuration.static_ip()),
      IPParser::parse_ipv4(configuration.default_gateway()),
      IPParser::parse_ipv4(configuration.subnet_mask())
      // IPParser::parse_ipv4("8.8.8.8"),  // Google DNS
    )) {
      DoLog.error("Failed to configure WiFi. Please check your configuration.", "wifi");
    } else {
      DoLog.verbose("Using DHCP ...", "wifi");
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
  SerialDebug.println("Firmware version: " + String(FIRMWARE_VERSION));
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

  // Once booted we should only use DoLog anymore!
  DoLog.set_destination(&SerialDebug);
  // DoLog.set_level(Logger::LogLevel::WARNING);

  // Setup WiFi (no need for reconnect Timer. WiFi lib has internal reconnect mechanism)
  // The rest is handled by the smart meter
  connect_to_wifi();

  // Start Smart Meter
  DoLog.info("Starting smart meter ... ", "boot");
  smartMeter.start(&configuration);
}

void loop() {
  smartMeter.process();
}
