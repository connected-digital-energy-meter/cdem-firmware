#include <ESP8266WiFi.h>

#define CDEM_PCB_V4
#include <CDEM.h>
#include <mqtt/pub_sub_publisher.h>
#include "version.h"

using namespace CDEM;

SmartDigitalMeter smartMeter(METER_REQUEST_PIN, &SerialMeter);
WiFiClient wifiClient;
PubSubPublisher publisher(wifiClient);    // Passed-by-reference
DeviceStatus deviceStatus(DATA_LED_PIN, COMM_LED_PIN);
ConfigurationManager configManager(EEPROM_CONFIG_ID, EEPROM_CONFIG_SIZE);

void connect_to_wifi(const Configuration * configuration) {
  DoLog.info("Connecting to WiFi ...", "wifi");
  WiFi.setOutputPower(OPERATIONAL_WIFI_OUTPUT_POWER);
  if (!configuration->use_dhcp()) {
    DoLog.verbose("Using static IP ...", "wifi");
    if (!WiFi.config(
      IPParser::parse_ipv4(configuration->static_ip()),
      IPParser::parse_ipv4(configuration->default_gateway()),
      IPParser::parse_ipv4(configuration->subnet_mask())
    )) {
      DoLog.error("Failed to configure WiFi. Please check your configuration.", "wifi");
    } else {
      DoLog.verbose("Using DHCP ...", "wifi");
    }
  }

  WiFi.begin(
    configuration->wifi_ssid().c_str(),
    configuration->wifi_password().c_str()
  );

  int i = 0;
  while (i++ < 10 && WiFi.status() != WL_CONNECTED) {
    delay(1000);
    DoLog.verbose("Connecting ...", "wifi");
  }

  if (WiFi.status() == WL_CONNECTED) DoLog.info("Successfully connected to WiFi", "wifi");
}

void configure_system() {
  // 1. Try loading from EEPROM
  DoLog.info("Loading configuration from EEPROM ...", "setup");
  if (!configManager.load_configuration()) {
    DoLog.warning("Configuration load from EEPROM failed. Loading factory defaults", "setup");
    configManager.factory_default();
  }

  // 2. Setup configuration portal
  DoLog.info("Setting up Captive Portal for configuration via WiFi", "setup");
  CaptivePortal * portal = new CaptivePortal(
    CAPTIVE_PORTAL_SSID,
    CAPTIVE_PORTAL_PASSWORD,
    CAPTIVE_PORTAL_TIME_WINDOW
  );
  portal->start(*configManager.current_config());
  deviceStatus.config_portal_up();

  while (!portal->process());   // Wait for Captive Portal to finish

  DoLog.verbose("Captive Portal finished ...", "setup");
  Configuration newConfiguration = portal->resulting_configuration();

  delete portal;

  // 3. Default = no boot; Altered = save to EEPROM
  if (newConfiguration == Configuration()) {
    DoLog.error("Not booting any further until device is properly configured.", "setup");
    deviceStatus.not_configured();
    while(true) { delay(1000); }
  } else if (newConfiguration != *(configManager.current_config())) {
    DoLog.verbose("Configuration has been altered via portal. Saving new config to EEPROM ...", "setup");
    configManager.current_config(newConfiguration);
    if (configManager.save_configuration()) {
      DoLog.info("Successfully saved new configuration to EEPROM", "setup");
    }   // What on fail ?
  }
}

void setup() {
  WiFi.setOutputPower(PORTAL_WIFI_OUTPUT_POWER);
  SerialDebug.begin(SERIAL_DEBUG_BAUDRATE);
  DoLog.set_destination(&SerialDebug);
  // DoLog.set_level(Logger::LogLevel::WARNING);
  delay(5000);    // Give some time to open serial terminal

  DoLog.info("\n\nStarting Connected Digital Energy Meter firmware ...", "setup");
  DoLog.info("Firmware version: " + String(FIRMWARE_VERSION));
  DoLog.info("CDEM Library version: " + String(CDEM_LIB_VERSION));

  configure_system();

  DoLog.info("Booting with configuration", "setup");
  DoLog.info("\n" + configManager.current_config()->to_string(), "setup");

  deviceStatus.clear();

  DoLog.info("Boot procedure finished", "setup");

  // Setup WiFi (no need for reconnect Timer. WiFi lib has internal reconnect mechanism)
  deviceStatus.connecting_wifi();
  connect_to_wifi(configManager.current_config());

  // Configure MQTT Publisher
  publisher.connect(
    configManager.current_config()->mqtt_broker(),
    configManager.current_config()->mqtt_port()
  );
  delay(1000);

  // Start Smart Meter
  DoLog.info("Starting smart meter ...", "setup");
  SerialMeter.begin(METER_BAUDRATE);
  smartMeter.set_publisher(&publisher);
  smartMeter.start(configManager.current_config(), &deviceStatus);
}

void loop() {
  smartMeter.process();
  yield();
}