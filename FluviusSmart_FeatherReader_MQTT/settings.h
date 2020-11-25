/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * General settings for P1 Reader program
 *
*/

// WIFI credentials
#define WIFI_SSID "IOTHOTSPOT"
#define WIFI_PASSWORD "GrIoT2020"

// Raspberri Pi Mosquitto MQTT Broker
#define MQTT_HOST "172.17.100.0"
#define MQTT_PORT 1883

// MQTT Topics
#define MQTT_C_L_TARIF "sensors/smartmeter/consumption_low_tarif"
#define MQTT_C_H_TARIF "sensors/smartmeter/consumption_high_tarif"
#define MQTT_P_L_TARIF "sensors/smartmeter/production_low_tarif"
#define MQTT_P_H_TARIF "sensors/smartmeter/production_high_tarif"
#define MQTT_C_T_POWER "sensors/smartmeter/total_power_consumption"
#define MQTT_P_T_POWER "sensors/smartmeter/total_power_production"
#define MQTT_A_TARIF "sensors/smartmeter/actual_tarif"
#define MQTT_C_GAS "sensors/smartmeter/gas_meter_m3"
#define MQTT_C_WATER "sensors/smartmeter/water_meter_m3"

// Baud rate for both hardware and software serial
#define SERIAL_DEBUG_BAUDRATE 115200
#define METER_BAUDRATE 115200

// Set DataRequest pin
const int STATE_LED = 13;
const int REQUEST_PIN = 14;

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
char datagramBuffer[1024] = { 0 };

// Set a readingpointer and startdetection
unsigned int readPointer = 0;
bool startDetected = false;

// Set to store the data values read
double CONSUMPTION_LOW_TARIF;
double CONSUMPTION_HIGH_TARIF;
double PRODUCTION_LOW_TARIF;
double PRODUCTION_HIGH_TARIF;
double TOTAL_POWER_CONSUMPTION;
double TOTAL_POWER_PRODUCTION;
double GAS_METER_M3;
double WATER_METER_M3;

// Set to store data counters read
double ACTUAL_TARIF;

// Set for periodic measurement
const long    period = 60000;
unsigned long startMillis;            
unsigned long currentMillis;


