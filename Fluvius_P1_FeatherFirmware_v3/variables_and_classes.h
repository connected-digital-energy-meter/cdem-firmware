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

// Baud rate for both hardware and software serial
#define SERIAL_DEBUG_BAUDRATE 115200
#define METER_BAUDRATE 115200

// Set DataRequest pin
const int REQUEST_PIN = 14;

// RGB led pins
#define DATA_LED_R 5    // SCK
#define DATA_LED_G 18   // MOSI
//#define DATA_LED_B 19   // MISO
#define DATA_LED_B 13
#define COMM_LED_R 27
#define COMM_LED_G 32
#define COMM_LED_B 33

// Define a serial for debugging and for reading the meter
#define SerialDebug Serial
#define SerialMeter Serial1

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
