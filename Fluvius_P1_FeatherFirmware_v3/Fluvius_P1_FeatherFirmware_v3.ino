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
#include <AsyncMqttClient.h>

// Include custom libraries
#include "rgb_led.h"
#include "variables_and_classes.h"
#include "digital_meter.h"

using namespace SmartMeter;

RgbLed commLed(COMM_LED_R, COMM_LED_G, COMM_LED_B, 1);
RgbLed dataLed(DATA_LED_R, DATA_LED_G, DATA_LED_B, 4);

Color NoWifiColor(Color::RED().dim(20));
Color NoMqttColor(Color::ORANGE().dim(20));
Color ComOkColor(Color::GREEN().dim(20));
Color EnableMeterColor(Color::BLUE().dim(20));
Color DisableMeterColor(Color::BLACK());

DigitalMeter meter(REQUEST_PIN, &SerialMeter, &SerialDebug);

// Connect to WiFi
void connectToWifi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// Connect to MQTT broker
void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
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
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      commLed.color(NoWifiColor);
      break;
  }
}

// On succesfull MQTT connection
void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  commLed.color(ComOkColor);
}

// On disconection from MQTT
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
  commLed.color(NoMqttColor);
}

void setup() {
  
  // Set the bautrate for both serial connections
  SerialDebug.begin(SERIAL_DEBUG_BAUDRATE);
  SerialMeter.begin(METER_BAUDRATE);

  // Pin Setup
  pinMode(REQUEST_PIN, OUTPUT);
  commLed.on();
  dataLed.on();
  commLed.color(NoWifiColor);

  // Clear outputs
  disable_meter();

  // Setup timers for WiFi and MQTT
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  // Setup eventhandler WiFi
  WiFi.onEvent(WiFiEvent);

  // Setup Eventhandlers MQTT
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onPublish(onMqttPublish);
  
  // Setup MQTT serverinfo
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");

  // Make Wifi Connection
  connectToWifi();

  // Wait until ESP is ready to receive info
  while ((!SerialDebug) && (millis() < 10000)) { }
  SerialDebug.println("Starting Feather Fluvius Meter Reader firmware ...");

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
        enable_meter();    
        break;
      // Read data
      case State::READING_DATAGRAM:
        if (meter.read_datagram(P1_data.datagramBuffer, 1024)) {
          currentState = State::DATAGRAM_READY;
        }
        // P1_data.read(currentState);
        break;
      // Stop requesting data & CRC check  
      case State::DATAGRAM_READY:        
        disable_meter();
        P1_data.crc_check(currentState);        
        break;
      // Decode data  
      case State::PROCESSING_DATA_GRAM:
        P1_data.decode(currentState);        
        break;
      // Publish data to MQTT  
      case State::DATAGRAM_DECODED:
        P1_data.publish();
        // Ready for next request    
        currentState = State::IDLE;
        // reset timer
        startMillis = currentMillis;
        break;
    }
  }
}

// Enable the meter sending
void enable_meter(void) {
  SerialDebug.println("Enabling meter");
  // digitalWrite(REQUEST_PIN, HIGH);
  meter.enable();
  dataLed.color(EnableMeterColor);
  currentState = State::READING_DATAGRAM;
}

// Disable the meter sending
void disable_meter(void) {
  SerialDebug.println("We have a DATAGRAM ready for processing");
  meter.disable();
  // digitalWrite(REQUEST_PIN, LOW);
  dataLed.color(DisableMeterColor);
}

