/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * General settings for P1 Reader program
 *
*/

#include "crccheck.h"

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
unsigned int readPointer = 0;
bool startDetected = false;

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

    // Read a new datagram from the P1 port
    void read(State &theState)
    {
        while(theState == State::READING_DATAGRAM){
            if (SerialMeter.available() > 0) {
                
                // get next byte for the P1 port
                char incomingByte = SerialMeter.read();
                
                // Look for the start of the datagram
                if (!startDetected && incomingByte == '/') {
                    startDetected = true;
                    SerialDebug.println("Detected start of a datagram");
                }

                // Ignore all data on serial port if start was not detected
                if (startDetected) {
                    datagramBuffer[readPointer++] = incomingByte;

                    // terug weg doen, is puur om te debuggen
                    SerialDebug.print(incomingByte);

                    // Look for the end of the datagram
                    if (incomingByte == '\n' && datagramBuffer[readPointer-7] == '!') {
                        SerialDebug.println("Read in full datagram");
                        theState = State::DATAGRAM_READY;
                        readPointer=0;
                        startDetected=false;
                    }
                    
                    // End of datagram not found
                    if (readPointer>1024) {
                        SerialDebug.println("Invalid Datagram > No end detected");
                        theState = State::IDLE;
                        datagramBuffer[1024]={0};
                        readPointer=0;
                        startDetected=false;
                    }
                }
            }
        }
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

    // Parse value of single OBIS reference
    double ParseDataValue(char* key, int datablock)
    {
        // look for the OBIS reference
        char* position = strstr(datagramBuffer, key);

        // if OBIS reference found
        if (position)
        {
            // look for the start position of the value for this OBIS reference
            char* start = position + strlen(key)+1;

            // if there are two value blocks then look for the start of the second one
            if(datablock==2){
            char* start = strchr(start,'(')+1;  
            }

            // look for the end position of the value for this OBIS reference
            char* end = strchr(start,'*')-1;

            // get the value of the OBIS reference
            char buffer[64] = {0};
            strncpy(buffer,start,end-start+1);

            // return the value
            return atof(buffer);
        } else {
        // return invalid if OBIS reference not found
        return -1;
        }
    }

    //Decode the datagram to value for each OBIS reference
    void decode(State &theState){
        
        // 1-0:1.8.2 = OBIS reference for electricity delivered to client in low tariff
        CONSUMPTION_LOW_TARIF = ParseDataValue("1-0:1.8.2",1);
        Serial.print("CONSUMPTION_LOW_TARIF = ");
        Serial.println(CONSUMPTION_LOW_TARIF);

        // 1-0:1.8.1 = OBIS reference for electricity delivered to client in high tariff
        CONSUMPTION_HIGH_TARIF = ParseDataValue("1-0:1.8.1",1);
        Serial.print("CONSUMPTION_HIGH_TARIF = ");
        Serial.println(CONSUMPTION_HIGH_TARIF);

        // 1-0:2.8.2 = OBIS reference for electricity delivered by client in low tariff
        PRODUCTION_LOW_TARIF = ParseDataValue("1-0:2.8.2",1);
        Serial.print("PRODUCTION_LOW_TARIF =");
        Serial.println(PRODUCTION_LOW_TARIF);

        // 1-0:2.8.1 = OBIS reference for electricity delivered by client in high tariff
        PRODUCTION_HIGH_TARIF = ParseDataValue("1-0:2.8.1",1);
        Serial.print("PRODUCTION_HIGH_TARIF =");
        Serial.println(PRODUCTION_HIGH_TARIF);

        // 1-0:1.7.0 = OBIS reference actual total power delivered to client
        TOTAL_POWER_CONSUMPTION = ParseDataValue("1-0:1.7.0",1);
        Serial.print("TOTAL_POWER_CONSUMPTION = ");
        Serial.println(TOTAL_POWER_CONSUMPTION);

        // 1-0:2.7.0 = OBIS reference actual total power delivered by client
        TOTAL_POWER_PRODUCTION = ParseDataValue("1-0:2.7.0",1);
        Serial.print("TOTAL_POWER_PRODUCTION = ");
        Serial.println(TOTAL_POWER_PRODUCTION);

        // 0-0:96.14.0 = OBIS reference actual tariff
        ACTUAL_TARIF = ParseDataValue("0-0:96.14.0",1);
        Serial.print("ACTUAL_TARIF = ");
        Serial.println(ACTUAL_TARIF);

        // 1-0:32.7.0 = OBIS reference actual voltage L1
        ACTUAL_VOLTAGE_L1 = ParseDataValue("1-0:32.7.0",1);
        Serial.print("ACTUAL_VOLTAGE_L1 =");
        Serial.println(ACTUAL_VOLTAGE_L1);

        // 1-0:52.7.0 = OBIS reference actual voltage L2
        ACTUAL_VOLTAGE_L2 = ParseDataValue("1-0:52.7.0",1);
        Serial.print("ACTUAL_VOLTAGE_L2 =");
        Serial.println(ACTUAL_VOLTAGE_L2);

        // 1-0:72.7.0 = OBIS reference actual voltage L3
        ACTUAL_VOLTAGE_L3 = ParseDataValue("1-0:72.7.0",1);
        Serial.print("ACTUAL_VOLTAGE_L3 =");
        Serial.println(ACTUAL_VOLTAGE_L3);

        // 1-0:31.7.0 = OBIS reference actual current L1
        ACTUAL_CURRENT_L1 = ParseDataValue("1-0:31.7.0",1);
        Serial.print("ACTUAL_CURRENT_L1 =");
        Serial.println(ACTUAL_CURRENT_L1);

        // 1-0:51.7.0 = OBIS reference actual current L2
        ACTUAL_CURRENT_L2 = ParseDataValue("1-0:51.7.0",1);
        Serial.print("ACTUAL_CURRENT_L2 =");
        Serial.println(ACTUAL_CURRENT_L2);
        
        // 1-0:71.7.0 = OBIS reference actual current L3
        ACTUAL_CURRENT_L3 = ParseDataValue("1-0:71.7.0",1);
        Serial.print("ACTUAL_CURRENT_L3 =");
        Serial.println(ACTUAL_CURRENT_L3);

        // 1-0:22.7.0 = OBIS reference actual power comsumption L1
        L1_POWER_CONSUMPTION = ParseDataValue("1-0:22.7.0",1);
        Serial.print("L1_POWER_COMSUMPTION = ");
        Serial.println(L1_POWER_CONSUMPTION);

        // 1-0:42.7.0 = OBIS reference actual power comsumption L2
        L2_POWER_CONSUMPTION = ParseDataValue("1-0:42.7.0",1);
        Serial.print("L2_POWER_COMSUMPTION = ");
        Serial.println(L2_POWER_CONSUMPTION);

        // 1-0:62.7.0 = OBIS reference actual power comsumption L3
        L3_POWER_CONSUMPTION = ParseDataValue("1-0:62.7.0",1);
        Serial.print("L3_POWER_COMSUMPTION = ");
        Serial.println(L3_POWER_CONSUMPTION);
  
        // 1-0:21.7.0 = OBIS reference actual power production L1
        L1_POWER_PRODUCTION = ParseDataValue("1-0:21.7.0",1);
        Serial.print("L1_POWER_PRODUCTION = ");
        Serial.println(L1_POWER_PRODUCTION);

        // 1-0:41.7.0 = OBIS reference actual power production L2
        L2_POWER_PRODUCTION = ParseDataValue("1-0:41.7.0",1);
        Serial.print("L2_POWER_PRODUCTION = ");
        Serial.println(L2_POWER_PRODUCTION);
        
        // 1-0:61.7.0 = OBIS reference actual power production L3
        L3_POWER_PRODUCTION = ParseDataValue("1-0:61.7.0",1);
        Serial.print("L3_POWER_PRODUCTION = ");
        Serial.println(L3_POWER_PRODUCTION);
        
        // 0-1:24.2.3 = OBIS reference gas delivered to client with temperature correction , 0-n where the n is the device number, possibly you need to change this number for your configuration
        GAS_METER_M3 = ParseDataValue("0-1:24.2.3",2);
        Serial.print("GAS_METER_M3 = ");
        Serial.println(GAS_METER_M3);
        
        // 0-2:24.2.1 = OBIS reference water delivered to client, 0-n where the n is the device number, possibly you need to change this number for your configuration
        WATER_METER_M3 = ParseDataValue("0-2:24.2.1",2);
        Serial.print("WATER_METER_M3 =");
        Serial.println(WATER_METER_M3); 

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

