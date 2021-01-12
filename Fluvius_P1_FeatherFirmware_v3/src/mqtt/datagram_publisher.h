/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
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
      DatagramPublisher(HardwareSerial * debugSerial=nullptr);

    public:
      void on_mqtt_event(std::function<void(MqttEvent)> mqttEventCallback);

    public:
      void connect(String host, uint16_t port);
      void disconnect(void);

    public:
      void publish(Datagram * _datagram);

    private:
      void setup_callbacks(void);

    private:
      void connect(void);

    private:
      void on_connected(bool sessionPresent);
      void on_disconnected(AsyncMqttClientDisconnectReason reason);

    private:
      void start_reconnect_timer(void);
      void stop_reconnect_timer(void);

    private:
      static void reconnect_timer_callback(TimerHandle_t timer);

    private:
      HardwareSerial * debugSerial;
      AsyncMqttClient mqttClient;
      bool _connected = false;
      bool _shouldBeConnected = false;
      TimerHandle_t reconnectTimer;
      static const unsigned int RECONNECT_TIME_MS = 10000;

      std::function<void(MqttEvent)> mqttEventCallback = nullptr;
  };

};
