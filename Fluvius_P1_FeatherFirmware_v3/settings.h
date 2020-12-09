/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * General settings for P1 Reader program
 *
*/

// Include class datagram
#include "datagram.h"

// WIFI credentials
#define WIFI_SSID "IOTHOTSPOT"          // Via webpage
#define WIFI_PASSWORD "GrIoT2020"       // Via webpage

// Raspberri Pi Mosquitto MQTT Broker
#define MQTT_HOST "172.17.100.2"        // Via webpage
#define MQTT_PORT 1883

// MQTT Topics
#define MQTT_C_L_TARIF "sensors/smartmeter/consumption_low_tarif"
#define MQTT_C_H_TARIF "sensors/smartmeter/consumption_high_tarif"
#define MQTT_P_L_TARIF "sensors/smartmeter/production_low_tarif"
#define MQTT_P_H_TARIF "sensors/smartmeter/production_high_tarif"
#define MQTT_C_T_POWER "sensors/smartmeter/total_power_consumption"
#define MQTT_P_T_POWER "sensors/smartmeter/total_power_production"
#define MQTT_A_TARIF "sensors/smartmeter/actual_tarif"
#define MQTT_V_L1 "sensors/smartmeter/actual_voltage_l1"
#define MQTT_V_L2 "sensors/smartmeter/actual_voltage_l2"
#define MQTT_V_L3 "sensors/smartmeter/actual_voltage_l3"
#define MQTT_A_L1 "sensors/smartmeter/actual_current_l1"
#define MQTT_A_L2 "sensors/smartmeter/actual_current_l2"
#define MQTT_A_L3 "sensors/smartmeter/actual_current_l3"
#define MQTT_C_L1_POWER "sensors/smartmeter/l1_power_consumption"
#define MQTT_C_L2_POWER "sensors/smartmeter/l2_power_consumption"
#define MQTT_C_L3_POWER "sensors/smartmeter/l3_power_consumption"
#define MQTT_P_L1_POWER "sensors/smartmeter/l1_power_production"
#define MQTT_P_L2_POWER "sensors/smartmeter/l2_power_production"
#define MQTT_P_L3_POWER "sensors/smartmeter/l3_power_production"
#define MQTT_C_GAS "sensors/smartmeter/gas_meter_m3"
#define MQTT_C_WATER "sensors/smartmeter/water_meter_m3"

// Baud rate for both hardware and software serial
#define SERIAL_DEBUG_BAUDRATE 115200
#define METER_BAUDRATE 115200

// Set DataRequest pin
const int STATE_LED = 13;
const int REQUEST_PIN = 14;
const int WIFI_LED = 12;  // Veranderen naar RGB
const int MQTT_LED = 11;  // Veranderen naar RGB

// Define a serial for debugging and for reading the meter
#define SerialDebug Serial
#define SerialMeter Serial1

// Define a program state class
enum class State {
  IDLE,
  READING_DATAGRAM,
  DATAGRAM_READY,
  PROCESSING_DATA_GRAM,
  DATAGRAM_DECODED
};

// Set to store received datagram
datagram P1_data;

// Set a readingpointer and startdetection
// unsigned int readPointer = 0;
// bool startDetected = false;

// Set to store the data values read
double CONSUMPTION_LOW_TARIF;
double CONSUMPTION_HIGH_TARIF;
double PRODUCTION_LOW_TARIF;
double PRODUCTION_HIGH_TARIF;
double TOTAL_POWER_CONSUMPTION;
double TOTAL_POWER_PRODUCTION;
double ACTUAL_VOLTAGE_L1;
double ACTUAL_VOLTAGE_L2;
double ACTUAL_VOLTAGE_L3;
double ACTUAL_CURRENT_L1;
double ACTUAL_CURRENT_L2;
double ACTUAL_CURRENT_L3;
double L1_POWER_CONSUMPTION;
double L2_POWER_CONSUMPTION;
double L3_POWER_CONSUMPTION;
double L1_POWER_PRODUCTION;
double L2_POWER_PRODUCTION;
double L3_POWER_PRODUCTION;
double GAS_METER_M3;
double WATER_METER_M3;

// Set to store data counters read
double ACTUAL_TARIF;

// Set for periodic measurement
const long    period = 10000;
unsigned long startMillis;            
unsigned long currentMillis;


