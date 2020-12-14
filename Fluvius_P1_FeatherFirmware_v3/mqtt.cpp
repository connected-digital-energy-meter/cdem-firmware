#include "mqtt.h"
#include "datagram.h"
#include "variables_and_classes.h"
#include <AsyncMqttClient.h>

namespace SmartMeter{

    void MqttService::publish(Datagram * _datagram,AsyncMqttClient * _mqttClient ){
        uint16_t packetIdPub1 = _mqttClient.publish(MQTT_C_L_TARIF, 1, true, String(_datagram.consumption_low_tarif()).c_str());                            
        uint16_t packetIdPub2 = _mqttClient.publish(MQTT_C_H_TARIF, 1, true, String(_datagram.consumption_high_tarif()).c_str());                            
        uint16_t packetIdPub3 = _mqttClient.publish(MQTT_P_L_TARIF, 1, true, String(_datagram.production_low_tarif()).c_str());                            
        uint16_t packetIdPub4 = _mqttClient.publish(MQTT_P_H_TARIF, 1, true, String(_datagram.production_high_tarif()).c_str());                            
        uint16_t packetIdPub5 = _mqttClient.publish(MQTT_C_T_POWER, 1, true, String(_datagram.total_power_consumption()).c_str());
        uint16_t packetIdPub9 = _mqttClient.publish(MQTT_P_T_POWER, 1, true, String(_datagram.total_power_production()).c_str());
        uint16_t packetIdPub13 = _mqttClient.publish(MQTT_A_TARIF, 1, true, String(_datagram.actual_tarif()).c_str());
        uint16_t packetIdPub14 = _mqttClient.publish(MQTT_V_L1, 1, true, String(_datagram.actual_voltage_l1()).c_str());
        uint16_t packetIdPub15 = _mqttClient.publish(MQTT_V_L2, 1, true, String(_datagram.actual_voltage_l2()).c_str());
        uint16_t packetIdPub16 = _mqttClient.publish(MQTT_V_L3, 1, true, String(_datagram.actual_voltage_l3()).c_str());
        uint16_t packetIdPub17 = _mqttClient.publish(MQTT_A_L1, 1, true, String(_datagram.actual_current_l1()).c_str());
        uint16_t packetIdPub18 = _mqttClient.publish(MQTT_A_L2, 1, true, String(_datagram.actual_current_l2()).c_str());
        uint16_t packetIdPub19 = _mqttClient.publish(MQTT_A_L3, 1, true, String(_datagram.actual_current_l3()).c_str());
        uint16_t packetIdPub20 = _mqttClient.publish(MQTT_C_L1_POWER, 1, true, String(_datagram.l1_power_consumption()).c_str());
        uint16_t packetIdPub21 = _mqttClient.publish(MQTT_C_L2_POWER, 1, true, String(_datagram.l2_power_consumption()).c_str());
        uint16_t packetIdPub22 = _mqttClient.publish(MQTT_C_L3_POWER, 1, true, String(_datagram.l3_power_consumption()).c_str());
        uint16_t packetIdPub23 = _mqttClient.publish(MQTT_P_L1_POWER, 1, true, String(_datagram.l1_power_production()).c_str());
        uint16_t packetIdPub24 = _mqttClient.publish(MQTT_P_L2_POWER, 1, true, String(_datagram.l2_power_production()).c_str());
        uint16_t packetIdPub25 = _mqttClient.publish(MQTT_P_L3_POWER, 1, true, String(_datagram.l3_power_production()).c_str());
        uint16_t packetIdPub26 = _mqttClient.publish(MQTT_C_GAS, 1, true, String(_datagram.gas_meter_m3()).c_str());                            
        uint16_t packetIdPub27 = _mqttClient.publish(MQTT_C_WATER, 1, true, String(_datagram.water_meter_m3()).c_str());
    }

}