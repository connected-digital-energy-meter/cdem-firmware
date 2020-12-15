#include "datagram_publisher.h"

namespace SmartMeter {
  // TODO - This should be refactored to include configurable base topic
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

  #define Debug(...) if(this->debugSerial) this->debugSerial->print(__VA_ARGS__); 
  #define DebugLn(...) if(this->debugSerial) this->debugSerial->println(__VA_ARGS__); 

  DatagramPublisher::DatagramPublisher(const char * host, uint16_t port, HardwareSerial * debugSerial) {
    this->debugSerial = debugSerial;
    mqttClient.setServer(host, port);
    setup_callbacks();

    // Pass instance pointer as ID to timer which can only be used with function/static method callbacks
    // Bit of a hack
    reconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(RECONNECT_TIME_MS), pdFALSE,
      (void*)this, &DatagramPublisher::reconnect_timer_callback);
  }

  void DatagramPublisher::setup_callbacks(void) {

    // std::function<void(bool sessionPresent)> OnConnectUserCallback;
    AsyncMqttClientInternals::OnConnectUserCallback onConnected
      = std::bind(&DatagramPublisher::on_connected, this, std::placeholders::_1 );
    
    mqttClient.onConnect(onConnected);

    // typedef std::function<void(AsyncMqttClientDisconnectReason reason)> OnDisconnectUserCallback;
    AsyncMqttClientInternals::OnDisconnectUserCallback onDisconnected
      = std::bind(&DatagramPublisher::on_disconnected, this, std::placeholders::_1 );
    
    mqttClient.onDisconnect(onDisconnected);
  }

  void DatagramPublisher::connect(void) {
    _shouldBeConnected = true;
    DebugLn("DGP - Connecting to MQTT broker");
    mqttClient.connect();
  }

  void DatagramPublisher::disconnect(void) {
    DebugLn("DGP - Disconnecting fromt MQTT broker");
    stop_reconnect_timer();
    _shouldBeConnected = false;
    mqttClient.disconnect();
  }

  void DatagramPublisher::on_connected(bool sessionPresent) {
    stop_reconnect_timer();
    _connected = true;
    DebugLn("DGP - Connected to MQTT broker");
  }

  void DatagramPublisher::on_disconnected(AsyncMqttClientDisconnectReason reason) {
    _connected = false;
    DebugLn("DGP - Disconnected from MQTT broker");
    if (_shouldBeConnected) {
      start_reconnect_timer();
    }
  }

  void DatagramPublisher::reconnect_timer_callback(TimerHandle_t timer) {
    auto self = (DatagramPublisher*)pvTimerGetTimerID(timer);
    self->connect();
  }

  void DatagramPublisher::start_reconnect_timer(void) {
    xTimerStart(reconnectTimer, 0);
  }

  void DatagramPublisher::stop_reconnect_timer(void) {
    xTimerStop(reconnectTimer, 0);
  }

  void DatagramPublisher::publish(Datagram * datagram) {
    mqttClient.publish(MQTT_C_L_TARIF, 1, true, String(datagram->consumption_low_tarif()).c_str());                            
    mqttClient.publish(MQTT_C_H_TARIF, 1, true, String(datagram->consumption_high_tarif()).c_str());                            
    mqttClient.publish(MQTT_P_L_TARIF, 1, true, String(datagram->production_low_tarif()).c_str());                            
    mqttClient.publish(MQTT_P_H_TARIF, 1, true, String(datagram->production_high_tarif()).c_str());                            
    mqttClient.publish(MQTT_C_T_POWER, 1, true, String(datagram->total_power_consumption()).c_str());
    mqttClient.publish(MQTT_P_T_POWER, 1, true, String(datagram->total_power_production()).c_str());
    mqttClient.publish(MQTT_A_TARIF, 1, true, String(datagram->actual_tarif()).c_str());
    mqttClient.publish(MQTT_V_L1, 1, true, String(datagram->actual_voltage_l1()).c_str());
    mqttClient.publish(MQTT_V_L2, 1, true, String(datagram->actual_voltage_l2()).c_str());
    mqttClient.publish(MQTT_V_L3, 1, true, String(datagram->actual_voltage_l3()).c_str());
    mqttClient.publish(MQTT_A_L1, 1, true, String(datagram->actual_current_l1()).c_str());
    mqttClient.publish(MQTT_A_L2, 1, true, String(datagram->actual_current_l2()).c_str());
    mqttClient.publish(MQTT_A_L3, 1, true, String(datagram->actual_current_l3()).c_str());
    mqttClient.publish(MQTT_C_L1_POWER, 1, true, String(datagram->l1_power_consumption()).c_str());
    mqttClient.publish(MQTT_C_L2_POWER, 1, true, String(datagram->l2_power_consumption()).c_str());
    mqttClient.publish(MQTT_C_L3_POWER, 1, true, String(datagram->l3_power_consumption()).c_str());
    mqttClient.publish(MQTT_P_L1_POWER, 1, true, String(datagram->l1_power_production()).c_str());
    mqttClient.publish(MQTT_P_L2_POWER, 1, true, String(datagram->l2_power_production()).c_str());
    mqttClient.publish(MQTT_P_L3_POWER, 1, true, String(datagram->l3_power_production()).c_str());
    mqttClient.publish(MQTT_C_GAS, 1, true, String(datagram->gas_meter_m3()).c_str());                            
    mqttClient.publish(MQTT_C_WATER, 1, true, String(datagram->water_meter_m3()).c_str());
  }

};