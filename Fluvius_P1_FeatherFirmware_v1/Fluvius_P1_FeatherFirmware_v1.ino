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
#include "settings.h"

// Initiate MQTT client
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

// Declare State and set state to IDLE
State currentState = State::IDLE;

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
      digitalWrite(WIFI_LED,HIGH);
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      digitalWrite(WIFI_LED, LOW);
      break;
  }
}

// On succesfull MQTT connection
void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  digitalWrite(MQTT_LED, HIGH;
}

// On disconection from MQTT
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
  digitalWrite(MQTT_LED, LOW);
}

void setup() {
  
  // Set the bautrate for both serial connections
  SerialDebug.begin(SERIAL_DEBUG_BAUDRATE);
  SerialMeter.begin(METER_BAUDRATE);

  // Pin Setup
  pinMode(STATE_LED, OUTPUT);
  pinMode(REQUEST_PIN, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(MQTT_LED,OUTPUT);

  // Clear outputs
  digitalWrite(STATE_LED, LOW);
  digitalWrite(WIFI_LED, LOW);
  digitalWrite(MQTT_LED, LOW);
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
    
  if((currentMillis - startMillis) >= period){
    if ((currentState == State::IDLE)) {
      
      // Request data
      digitalWrite(STATE_LED, LOW);
      SerialDebug.println("Enabling meter");
      enable_meter();    
      currentState = State::READING_DATAGRAM;

    } else if (currentState == State::READING_DATAGRAM) {
      
      // Read data
      read_datagram();
    } else if (currentState == State::DATAGRAM_READY) {
      // Stop requesting data
      digitalWrite(STATE_LED, HIGH);
      SerialDebug.println("We have a DATAGRAM ready for processing");
      disable_meter();

      // CRC16 check
      if(Crc16()){
        currentState = State::PROCESSING_DATA_GRAM;  
      } else {
        SerialDebug.println("CRC16 check failed");
        currentState = State::IDLE;
      }
    } else if (currentState == State::PROCESSING_DATA_GRAM){  
      
      // Decode data
      if(decode_datagram()){
        currentState = State::DATAGRAM_DECODED;
      } else {
        currentState = State::IDLE;
      }
    } else if (currentState == State::DATAGRAM_DECODED){  
      // Debugging
      debug_received_data();
      
      // Publish data to MQTT
      publish_received_data();
      
      // Ready for next request    
      currentState = State::IDLE;
      digitalWrite(STATE_LED, LOW);
      
      // reset timer
      startMillis = currentMillis;

    }
  }
}

// Publish received data to MQTT
void publish_received_data(){
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

// Send received data to serial monitor for debugging
void debug_received_data(){
  Serial.print("CONSUMPTION_LOW_TARIF = ");
  Serial.println(CONSUMPTION_LOW_TARIF);
  Serial.print("CONSUMPTION_HIGH_TARIF = ");
  Serial.println(CONSUMPTION_HIGH_TARIF);
  Serial.print("PRODUCTION_LOW_TARIF =");
  Serial.println(PRODUCTION_LOW_TARIF);
  Serial.print("PRODUCTION_HIGH_TARIF =");
  Serial.println(PRODUCTION_HIGH_TARIF);
  Serial.print("TOTAL_POWER_CONSUMPTION = ");
  Serial.println(TOTAL_POWER_CONSUMPTION);
  Serial.print("TOTAL_POWER_PRODUCTION = ");
  Serial.println(TOTAL_POWER_PRODUCTION);
  Serial.print("ACTUAL_TARIF = ");
  Serial.println(ACTUAL_TARIF);
  Serial.print("ACTUAL_VOLTAGE_L1 =");
  Serial.println(ACTUAL_VOLTAGE_L1);
  Serial.print("ACTUAL_VOLTAGE_L2 =");
  Serial.println(ACTUAL_VOLTAGE_L2);
  Serial.print("ACTUAL_VOLTAGE_L3 =");
  Serial.println(ACTUAL_VOLTAGE_L3);
  Serial.print("ACTUAL_CURRENT_L1 =");
  Serial.println(ACTUAL_CURRENT_L1);
  Serial.print("ACTUAL_CURRENT_L2 =");
  Serial.println(ACTUAL_CURRENT_L2);
  Serial.print("ACTUAL_CURRENT_L3 =");
  Serial.println(ACTUAL_CURRENT_L3);
  Serial.print("L1_POWER_COMSUMPTION = ");
  Serial.println(L1_POWER_CONSUMPTION);
  Serial.print("L2_POWER_COMSUMPTION = ");
  Serial.println(L2_POWER_CONSUMPTION);
  Serial.print("L3_POWER_COMSUMPTION = ");
  Serial.println(L3_POWER_CONSUMPTION);
  Serial.print("L1_POWER_PRODUCTION = ");
  Serial.println(L1_POWER_PRODUCTION);
  Serial.print("L2_POWER_PRODUCTION = ");
  Serial.println(L2_POWER_PRODUCTION);
  Serial.print("L3_POWER_PRODUCTION = ");
  Serial.println(L3_POWER_PRODUCTION);
  Serial.print("GAS_METER_M3 = ");
  Serial.println(GAS_METER_M3);
  Serial.print("WATER_METER_M3 =");
  Serial.println(WATER_METER_M3); 
}

// Enable the meter sending
void enable_meter(void) {
  digitalWrite(REQUEST_PIN, HIGH);
}

// Disable the meter sending
void disable_meter(void) {
  digitalWrite(REQUEST_PIN, LOW);
}

// Read the next byte of the telegram
void read_datagram(void) {
  if (SerialMeter.available() > 0) {
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
        readPointer=0;
        startDetected=false;
      }
    }
  }
}

// Decode the telegram
bool decode_datagram()
{
    // Find the start of the datagram
    char* startChar = strstr(datagramBuffer, "/");

    // Find the end of the datagram
    char* endChar = strstr(datagramBuffer, "!");
    
    // If the end or start are not found we have a invalid datagram
    if(!startChar || !endChar) {
        Serial.println("Invalid datagram");
        return false;
    } 

    // 1-0:1.8.2 = OBIS reference for electricity delivered to client in low tariff
    CONSUMPTION_LOW_TARIF = ParseDataValue("1-0:1.8.2",1);
    
    // 1-0:1.8.1 = OBIS reference for electricity delivered to client in high tariff
    CONSUMPTION_HIGH_TARIF = ParseDataValue("1-0:1.8.1",1);

    // 1-0:2.8.2 = OBIS reference for electricity delivered by client in low tariff
    PRODUCTION_LOW_TARIF = ParseDataValue("1-0:2.8.2",1);

    // 1-0:2.8.1 = OBIS reference for electricity delivered by client in high tariff
    PRODUCTION_HIGH_TARIF = ParseDataValue("1-0:2.8.1",1);
    
    // 1-0:1.7.0 = OBIS reference actual total power delivered to client
    TOTAL_POWER_CONSUMPTION = ParseDataValue("1-0:1.7.0",1);

    // 1-0:2.7.0 = OBIS reference actual total power delivered by client
    TOTAL_POWER_PRODUCTION = ParseDataValue("1-0:2.7.0",1);

    // 0-0:96.14.0 = OBIS reference actual tariff
    ACTUAL_TARIF = ParseDataValue("0-0:96.14.0",1);

    // 1-0:32.7.0 = OBIS reference actual voltage L1
    ACTUAL_VOLTAGE_L1 - ParseDataValue("1-0:32.7.0");

    // 1-0:52.7.0 = OBIS reference actual voltage L2
    ACTUAL_VOLTAGE_L2 - ParseDataValue("1-0:52.7.0");

    // 1-0:72.7.0 = OBIS reference actual voltage L3
    ACTUAL_VOLTAGE_L3 - ParseDataValue("1-0:72.7.0");

    // 1-0:31.7.0 = OBIS reference actual current L1
    ACTUAL_CURRENT_L1 - ParseDataValue("1-0:31.7.0");

    // 1-0:51.7.0 = OBIS reference actual current L2
    ACTUAL_CURRENT_L2 - ParseDataValue("1-0:51.7.0");
    
    // 1-0:71.7.0 = OBIS reference actual current L3
    ACTUAL_CURRENT_L3 - ParseDataValue("1-0:71.7.0");
    
    // 1-0:22.7.0 = OBIS reference actual power comsumption L1
    L1_POWER_CONSUMPTION - ParseDataValue("1-0:22.7.0");

    // 1-0:42.7.0 = OBIS reference actual power comsumption L2
    L2_POWER_CONSUMPTION - ParseDataValue("1-0:42.7.0");
    
    // 1-0:62.7.0 = OBIS reference actual power comsumption L3
    L3_POWER_CONSUMPTION - ParseDataValue("1-0:62.7.0");

    // 1-0:21.7.0 = OBIS reference actual power production L1
    L1_POWER_PRODUCTION - ParseDataValue("1-0:21.7.0");

    // 1-0:41.7.0 = OBIS reference actual power production L2
    L2_POWER_PRODUCTION - ParseDataValue("1-0:41.7.0");

    // 1-0:61.7.0 = OBIS reference actual power production L3
    L3_POWER_PRODUCTION - ParseDataValue("1-0:61.7.0");
    
    // 0-1:24.2.3 = OBIS reference gas delivered to client with temperature correction , 0-n where the n is the device number, possibly you need to change this number for your configuration
    GAS_METER_M3 = ParseDataValue("0-1:24.2.3",2);

    // 0-2:24.2.1 = OBIS reference water delivered to client, 0-n where the n is the device number, possibly you need to change this number for your configuration
    WATER_METER_M3 = ParseDataValue("0-2:24.2.1",2);
    return true;
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

bool Crc16(){
  // Start with a fase validation
  bool validation=false;

  // Find the start of the datagram
  char* startChar = strstr(datagramBuffer, "/");

  // Find the end of the datagram
  char* endChar = strstr(datagramBuffer, "!");

  // Get the actual datagram
  char datagram[1024] = datagramBuffer.substring(startChar,endChar+1);

  // Get the CRC checksum
  char crccheck[4] = datagramBuffer.substring(endChar+1,endChar+5);

  // Starting crc
  char crc[4] = 0;

  // crc calculaton for each char position
  for(pos=startChar;pos<=endChar;pos++){

    crc ^= (unsigned int)datagram[pos];    // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;                // Polynomial
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }

  // If crc matches the crcchecksum at the and of the datagram we have a valid crc
  if(crc==crccheck) validation=true;

  return validation;
}