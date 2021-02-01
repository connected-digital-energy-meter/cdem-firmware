/*
 * VIVES college Research
 *
 * Author Ronny Mees & Nico De Witte
 *
 * Class to publish the P1 data to a MQTT broker
 *
*/
#pragma once

#include <Arduino.h>
#include <AsyncMqttClient.h>
#include "../digital_meter/datagram.h"

namespace SmartMeter {

  class DatagramPublisher {

    public:
      enum class MqttEvent {
        CONNECTED, DISCONNECTED
      };

    public:
      DatagramPublisher(void);
      ~DatagramPublisher(void);

    public:
      void connect(String host, uint16_t port);
      void disconnect(void);
      bool publish(Datagram * _datagram, String baseTopic);
      bool connected(void);

    public:
      void on_mqtt_event(std::function<void(MqttEvent)> mqttEventCallback);

    private:
      void setup_callbacks(void);
      void _connect(void);

    private:
      void on_connected(bool sessionPresent);
      void on_disconnected(AsyncMqttClientDisconnectReason reason);
      void on_published(uint16_t packetId);

    private:
      AsyncMqttClient mqttClient;
      std::function<void(MqttEvent)> mqttEventCallback = nullptr;
  };

};