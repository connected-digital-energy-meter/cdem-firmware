/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * General settings for P1 Reader program
 *
*/

#include "crccheck.h"
#include "datagram.h"
#include "decoder.h"

// WIFI credentials
#define WIFI_SSID "IOTHOTSPOT"          // Via webpage
#define WIFI_PASSWORD "GrIoT2020"       // Via webpage

// Raspberri Pi Mosquitto MQTT Broker
#define MQTT_HOST "172.17.100.3"        // Via webpage
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

// Define a program state class
enum class State {
  IDLE,
  READING_DATAGRAM,
  DATAGRAM_READY,
  PROCESSING_DATA_GRAM,
  DATAGRAM_DECODED
};

// Initiate MQTT client
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

// Declare State and set state to IDLE
State currentState = State::IDLE;

// Class datagram
class datagram
{
    public:
    // variabelen
    char datagramBuffer[1024];

    // constructor
    datagram(){
        datagramBuffer[1024] = { 0 };
    }

    // Do a CRC16-IBM check for validation of the received datagram
    void crc_check(State &theState){
        
        if(SmartMeter::CRCchecker::checkCRC(datagramBuffer,sizeof(datagramBuffer))){
            theState = State::PROCESSING_DATA_GRAM; 
        } else {
            SerialDebug.println("CRC16 check failed");
            theState = State::IDLE;
            datagramBuffer[1024]={0};
        }
        
    }

    //Decode the datagram to value for each OBIS reference
    void decode(State &theState){
        
        Datagram test = SmartMeter::Decode::decode(datagramBuffer,sizeof(datagramBuffer));
        
        // End of decoding
        theState = State::DATAGRAM_DECODED;
    }

    void publish(){
        uint16_t packetIdPub1 = mqttClient.publish(MQTT_C_L_TARIF, 1, true, String(CONSUMPTION_LOW_TARIF).c_str());                            
        uint16_t packetIdPub2 = mqttClient.publish(MQTT_C_H_TARIF, 1, true, String(CONSUMPTION_HIGH_TARIF).c_str());                            
        uint16_t packetIdPub3 = mqttClient.publish(MQTT_P_L_TARIF, 1, true, String(PRODUCTION_LOW_TARIF).c_str());                            
        uint16_t packetIdPub4 = mqttClient.publish(MQTT_P_H_TARIF, 1, true, String(PRODUCTION_HIGH_TARIF).c_str());                            
        uint16_t packetIdPub5 = mqttClient.publish(MQTT_C_T_POWER, 1, true, String(TOTAL_POWER_CONSUMPTION).c_str());
        uint16_t packetIdPub9 = mqttClient.publish(MQTT_P_T_POWER, 1, true, String(TOTAL_POWER_PRODUCTION).c_str());
        uint16_t packetIdPub13 = mqttClient.publish(MQTT_A_TARIF, 1, true, String(ACTUAL_TARIF).c_str());
        uint16_t packetIdPub14 = mqttClient.publish(MQTT_V_L1, 1, true, String(ACTUAL_VOLTAGE_L1).c_str());
        uint16_t packetIdPub15 = mqttClient.publish(MQTT_V_L2, 1, true, String(ACTUAL_VOLTAGE_L2).c_str());
        uint16_t packetIdPub16 = mqttClient.publish(MQTT_V_L3, 1, true, String(ACTUAL_VOLTAGE_L3).c_str());
        uint16_t packetIdPub17 = mqttClient.publish(MQTT_A_L1, 1, true, String(ACTUAL_CURRENT_L1).c_str());
        uint16_t packetIdPub18 = mqttClient.publish(MQTT_A_L2, 1, true, String(ACTUAL_CURRENT_L2).c_str());
        uint16_t packetIdPub19 = mqttClient.publish(MQTT_A_L3, 1, true, String(ACTUAL_CURRENT_L3).c_str());
        uint16_t packetIdPub20 = mqttClient.publish(MQTT_C_L1_POWER, 1, true, String(L1_POWER_CONSUMPTION).c_str());
        uint16_t packetIdPub21 = mqttClient.publish(MQTT_C_L2_POWER, 1, true, String(L2_POWER_CONSUMPTION).c_str());
        uint16_t packetIdPub22 = mqttClient.publish(MQTT_C_L3_POWER, 1, true, String(L3_POWER_CONSUMPTION).c_str());
        uint16_t packetIdPub23 = mqttClient.publish(MQTT_P_L1_POWER, 1, true, String(L1_POWER_PRODUCTION).c_str());
        uint16_t packetIdPub24 = mqttClient.publish(MQTT_P_L2_POWER, 1, true, String(L2_POWER_PRODUCTION).c_str());
        uint16_t packetIdPub25 = mqttClient.publish(MQTT_P_L3_POWER, 1, true, String(L3_POWER_PRODUCTION).c_str());
        uint16_t packetIdPub26 = mqttClient.publish(MQTT_C_GAS, 1, true, String(GAS_METER_M3).c_str());                            
        uint16_t packetIdPub27 = mqttClient.publish(MQTT_C_WATER, 1, true, String(WATER_METER_M3).c_str());
        Serial.println("Published to MQTT");          
    }  
};

// Set to store received datagram
datagram P1_data;

