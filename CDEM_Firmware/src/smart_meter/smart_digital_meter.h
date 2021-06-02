#pragma once

#include "../digital_meter/datagram.h"
#include "../digital_meter/digital_meter.h"
#include "../mqtt/datagram_publisher.h"
#include "../config/configuration.h"
#include "../status/device_status.h"
#include <WiFi.h>

namespace SmartMeter {

  class SmartDigitalMeter {

    public:
      SmartDigitalMeter(DeviceStatus * deviceStatus);

    public:
      void start(Configuration * config);
      void stop(void);
      void process(void);   // Call this in loop()

    private:
      void wifi_event(WiFiEvent_t event);
      void on_mqtt_event(DatagramPublisher::MqttEvent event);

    private:
      void communications_check(void);
      void setup_publisher(void);
      void destroy_publisher(void);

    private:
      // Define a program state class
      enum class State {
        IDLE,
        READING_DATAGRAM,
        DATAGRAM_READY,
        PROCESSING_DATAGRAM,
        DATAGRAM_DECODED
      };

      enum class CommState {
        AWAITING_WIFI,
        SETUP_MQTT,
        AWAITING_MQTT,
        ALL_OPERATIONAL,
        WIFI_LOST
      };

    private:
      char datagramBuffer[1024] = {0};
      Datagram datagram;
      DigitalMeter meter;
      DatagramPublisher * publisher = nullptr;
      bool acquireData = false;
      bool mqttPublisherNeedsRebuild = false;

      // Set for periodic measurement
      long period = 10000L;
      unsigned long startMillis;            
      unsigned long currentMillis;
      unsigned long timeout;

      // Declare State and set state to IDLE
      State currentState = State::IDLE;
      CommState commState = CommState::AWAITING_WIFI;
      int mqttTimeoutCounter = 0;
      unsigned long lastCommCheck = 0;
      int wifiTimeoutCounter = 0;

      DeviceStatus * deviceStatus;
      Configuration * deviceConfig;
  };

};