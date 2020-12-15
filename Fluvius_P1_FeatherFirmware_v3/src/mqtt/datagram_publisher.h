#pragma once

#include <Arduino.h>
#include <AsyncMqttClient.h>
#include "../digital_meter/datagram.h"

namespace SmartMeter {

  class DatagramPublisher {

    public:
      DatagramPublisher(const char * host, uint16_t port, HardwareSerial * debugSerial=nullptr);

    public:
      void connect(void);
      void disconnect(void);

    public:
      void publish(Datagram * _datagram);

    private:
      void setup_callbacks(void);

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
  };

};
