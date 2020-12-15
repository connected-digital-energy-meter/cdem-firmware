/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * General settings for P1 Reader program
 *
*/
#pragma once

// WIFI credentials
#define WIFI_SSID "IOTHOTSPOT"          // Via webpage
#define WIFI_PASSWORD "GrIoT2020"       // Via webpage

// Raspberri Pi Mosquitto MQTT Broker
#define MQTT_HOST "172.17.100.3"        // Via webpage
#define MQTT_PORT 1883

// Set for periodic measurement
const long    period = 10000;
unsigned long startMillis;            
unsigned long currentMillis;

// Define a program state class
enum class State {
  IDLE,
  READING_DATAGRAM,
  DATAGRAM_READY,
  PROCESSING_DATA_GRAM,
  DATAGRAM_DECODED
};

TimerHandle_t wifiReconnectTimer;

// Declare State and set state to IDLE
State currentState = State::IDLE;
