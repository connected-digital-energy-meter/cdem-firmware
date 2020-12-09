/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * datagram class for P1 Reader program
 *
*/

#include "settings.h"

class datagram
{
    public:
    // variabelen
    char datagramBuffer[1024];

    // constructor
    datagram(){
        datagramBuffer = { 0 };
    }

    // Read a new datagram from the P1 port
    void read()
    {
        while(currentState == State::READING_DATAGRAM){
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
                        currentState = State::DATAGRAM_READY;
                        readPointer=0;
                        startDetected=false;
                    }
                    
                    // End of datagram not found
                    if (readPointer>1024) {
                        SerialDebug.println("Invalid Datagram > No end detected");
                        currentState = State::IDLE;
                        datagramBuffer={0};
                        readPointer=0;
                        startDetected=false;
                    }
                }
            }
        }
    }

    int getposition(const char* array, size_t size, char c)
    {
        const char* end = array + size;
        const char* match = std::find(array, end, c);
        return (end == match) ? -1 : (match - array);
    } 

    // Do a CRC16-IBM check for validation of the received datagram
    void crc_check(){
        
        // Find the start of the datagram
        int begin = getposition(datagramBuffer, sizeof(datagramBuffer), '/');
                
        // Find the end of the datagram
        int eind = getposition(datagramBuffer, sizeof(datagramBuffer), '!');
                    
        // Find the crc validation
        unsigned int crc_validation = (unsigned int)datagramBuffer.substring(eind+1,eind+5);

        // Starting crc check
        unsigned int crc = 0;
        int positie = begin;

        while (positie <= eind)
        {
            crc ^= (unsigned int)datagramBuffer[positie];
            int bit = 0;
            while (bit < 8) {
                if ((crc & 1) != 0) {
                    crc = (crc >> 1) ^ 0xA001;
                }
                else crc >>= 1;
                bit++;
            }
            positie++;
        }

        if(crc == crc_validation ){
            currentState = State::PROCESSING_DATA_GRAM; 
        } else {
            SerialDebug.println("CRC16 check failed");
            currentState = State::IDLE;
            datagramBuffer={0};
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
    void decode(){
        
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
        ACTUAL_VOLTAGE_L1 - ParseDataValue("1-0:32.7.0",1);
        Serial.print("ACTUAL_VOLTAGE_L1 =");
        Serial.println(ACTUAL_VOLTAGE_L1);

        // 1-0:52.7.0 = OBIS reference actual voltage L2
        ACTUAL_VOLTAGE_L2 - ParseDataValue("1-0:52.7.0",1);
        Serial.print("ACTUAL_VOLTAGE_L2 =");
        Serial.println(ACTUAL_VOLTAGE_L2);

        // 1-0:72.7.0 = OBIS reference actual voltage L3
        ACTUAL_VOLTAGE_L3 - ParseDataValue("1-0:72.7.0",1);
        Serial.print("ACTUAL_VOLTAGE_L3 =");
        Serial.println(ACTUAL_VOLTAGE_L3);

        // 1-0:31.7.0 = OBIS reference actual current L1
        ACTUAL_CURRENT_L1 - ParseDataValue("1-0:31.7.0",1);
        Serial.print("ACTUAL_CURRENT_L1 =");
        Serial.println(ACTUAL_CURRENT_L1);

        // 1-0:51.7.0 = OBIS reference actual current L2
        ACTUAL_CURRENT_L2 - ParseDataValue("1-0:51.7.0",1);
        Serial.print("ACTUAL_CURRENT_L2 =");
        Serial.println(ACTUAL_CURRENT_L2);
        
        // 1-0:71.7.0 = OBIS reference actual current L3
        ACTUAL_CURRENT_L3 - ParseDataValue("1-0:71.7.0",1);
        Serial.print("ACTUAL_CURRENT_L3 =");
        Serial.println(ACTUAL_CURRENT_L3);

        // 1-0:22.7.0 = OBIS reference actual power comsumption L1
        L1_POWER_CONSUMPTION - ParseDataValue("1-0:22.7.0",1);
        Serial.print("L1_POWER_COMSUMPTION = ");
        Serial.println(L1_POWER_CONSUMPTION);

        // 1-0:42.7.0 = OBIS reference actual power comsumption L2
        L2_POWER_CONSUMPTION - ParseDataValue("1-0:42.7.0",1);
        Serial.print("L2_POWER_COMSUMPTION = ");
        Serial.println(L2_POWER_CONSUMPTION);

        // 1-0:62.7.0 = OBIS reference actual power comsumption L3
        L3_POWER_CONSUMPTION - ParseDataValue("1-0:62.7.0",1);
        Serial.print("L3_POWER_COMSUMPTION = ");
        Serial.println(L3_POWER_CONSUMPTION);
  
        // 1-0:21.7.0 = OBIS reference actual power production L1
        L1_POWER_PRODUCTION - ParseDataValue("1-0:21.7.0",1);
        Serial.print("L1_POWER_PRODUCTION = ");
        Serial.println(L1_POWER_PRODUCTION);

        // 1-0:41.7.0 = OBIS reference actual power production L2
        L2_POWER_PRODUCTION - ParseDataValue("1-0:41.7.0",1);
        Serial.print("L2_POWER_PRODUCTION = ");
        Serial.println(L2_POWER_PRODUCTION);
        
        // 1-0:61.7.0 = OBIS reference actual power production L3
        L3_POWER_PRODUCTION - ParseDataValue("1-0:61.7.0",1);
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
        currentState = State::DATAGRAM_DECODED;
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