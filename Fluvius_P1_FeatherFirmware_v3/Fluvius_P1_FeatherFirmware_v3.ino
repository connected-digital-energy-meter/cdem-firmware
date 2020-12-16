/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * Program to read the P1 port of a Fluvius smart meter with a Feather Huzzah32
 *
*/

// Include general libraries
#include <cstring>
#include <WiFi.h>

// Include custom libraries
#include "hardware.h"
#include "src/rgb/rgb_led.h"
#include "variables_and_classes.h"
#include "src/digital_meter/datagram.h"
#include "src/digital_meter/digital_meter.h"
#include "src/digital_meter/decoder.h"
#include "src/mqtt/datagram_publisher.h"
#include "src/config/config_serializer.h"
#include "src/config/config_manager.h"

// Using the namespace SmartMeter
using namespace SmartMeter;

// Setup the RGB led's on the PCB
RgbLed commLed(COMM_LED_R, COMM_LED_G, COMM_LED_B, 1);

// Predefine some colors
Color NoWifiColor(Color::RED().dim(20));
Color NoMqttColor(Color::ORANGE().dim(20));
Color ComOkColor(Color::GREEN().dim(20));

// Initiate variable of Digitalmeter.
DigitalMeter meter(REQUEST_PIN, &SerialMeter, &SerialDebug);
DatagramPublisher publisher(MQTT_HOST, MQTT_PORT, &SerialDebug);
ConfigManager configManager;

// Initialise temporary datagrambuffer and datagram 
char datagramBuffer[1024] = {0};
SmartMeter::Datagram datagram;

// Connect to WiFi
void connectToWifi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// Connect to MQTT broker
void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  publisher.connect();
}

// Handle WiFi events
void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      commLed.color(NoMqttColor);
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStart(wifiReconnectTimer, 0);
      commLed.color(NoWifiColor);
      break;
  }
}

void setup() {
  // Set the baudrate for both serial connections
  SerialDebug.begin(SERIAL_DEBUG_BAUDRATE);
  SerialMeter.begin(METER_BAUDRATE);

  SerialDebug.println("Starting Feather Fluvius Meter Reader firmware ...");

  // Pin Setup for request data
  pinMode(REQUEST_PIN, OUTPUT);

  // Initiate the led's on the pcb
  commLed.on();
  commLed.color(NoWifiColor);

  // Stop requesting data from the meter
  meter.disable();

  SerialDebug.println("Loading configuration ...");
  if (configManager.load_configuration()) {
    SerialDebug.println("Loaded existing configuration");
  } else {
    SerialDebug.println("No existing configuration could be loaded");
    SerialDebug.println("Saving default configuration");
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

  // Setup timers for WiFi and MQTT
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  // Setup eventhandler WiFi
  WiFi.onEvent(WiFiEvent);

  // Make Wifi Connection
  connectToWifi();

  // Start time for period
  startMillis = millis();
}

void loop() {

  // Current time for period
  currentMillis = millis();

  // Wait until next period  
  if((currentMillis - startMillis) >= period){

    switch (currentState){
      // Request data
      case State::IDLE:
        meter.enable();
        currentState = State::READING_DATAGRAM;
        break;
      // Read data
      case State::READING_DATAGRAM:
        if (meter.read_datagram(datagramBuffer, sizeof(datagramBuffer))) {
          currentState = State::DATAGRAM_READY;
        }
        break;
      // Stop requesting data
      case State::DATAGRAM_READY:        
        meter.disable();
        currentState = State::PROCESSING_DATA_GRAM;
        break;
      // Decode data  
      case State::PROCESSING_DATA_GRAM:
        datagram = SmartMeter::Decoder::decode(datagramBuffer, sizeof(datagramBuffer));
        SerialDebug.println("Decoded datagram:");
        SerialDebug.println(datagram.to_string());
        currentState = State::DATAGRAM_DECODED;
        break;
      // Publish data to MQTT  
      case State::DATAGRAM_DECODED:
        publisher.publish(&datagram);
        SerialDebug.println("Datagram published");
        // Ready for next request    
        currentState = State::IDLE;
        // reset timer
        startMillis = currentMillis;
        break;
    }
  }
}





