#include "smart_digital_meter.h"

#include "../digital_meter/decoder.h"
#include "../../hardware.h"
#include "../logging/logger.h"

namespace SmartMeter {

  SmartDigitalMeter::SmartDigitalMeter(DeviceStatus * deviceStatus)
    : deviceStatus(deviceStatus),
      meter(REQUEST_PIN, deviceStatus, &SerialMeter) {

    SerialMeter.begin(METER_BAUDRATE);
    meter.disable();
    // WiFi.onEvent(std::bind(&SmartDigitalMeter::wifi_event, this, std::placeholders::_1 ));
  }

  void SmartDigitalMeter::start(Configuration * config) {
    this->period = config->read_period() * 1000;
    this->deviceConfig = config;
    
    deviceStatus->meter_starting();
    acquireData = true;

    // Start time for period
    startMillis = millis();
    lastCommCheck = startMillis;
  }

  void SmartDigitalMeter::stop(void) {
    acquireData = false;
  }

  void SmartDigitalMeter::process(void) {
    // Current time for period
    currentMillis = millis();

    // Can't do this event based as it triggers
    // endless DISCONNECT - RECONNECT events. This is simpler
    if ((currentMillis - lastCommCheck) >= 5000) {
      communications_check();
      lastCommCheck = millis();
    }

    // Wait until next period  
    if(acquireData && (currentMillis - startMillis) >= period) {

      switch (currentState) {
        // Request data
        case State::IDLE:
          meter.enable();
          timeout = millis();
          currentState = State::READING_DATAGRAM;
          break;
        // Read data
        case State::READING_DATAGRAM:
          if (meter.read_datagram(datagramBuffer, sizeof(datagramBuffer))) {
            currentState = State::DATAGRAM_READY;
          } else if( (millis()-timeout) > 1000) {
            meter.timeout();            
            currentState = State::IDLE;
            startMillis = currentMillis;
          }
          break;
        // Stop requesting data
        case State::DATAGRAM_READY:
          meter.disable();
          currentState = State::PROCESSING_DATAGRAM;
          break;
        // Decode data  
        case State::PROCESSING_DATAGRAM:
          datagram = SmartMeter::Decoder::decode(datagramBuffer, sizeof(datagramBuffer));
          DoLog.verbose(datagram.to_string(), "smart");
          currentState = State::DATAGRAM_DECODED;
          break;
        // Publish data to MQTT  
        case State::DATAGRAM_DECODED:
          if (publisher) publisher->publish(&datagram, deviceConfig->mqtt_topic());
          // Ready for next request    
          currentState = State::IDLE;
          // reset timer
          startMillis = currentMillis;
          break;
      }
    } else {
      delay(100);  // Gives other tasks some time
    }
  }

  // Let's just use this one for logging
  // WARNING - Serial is probable thread-safe.
  // void SmartDigitalMeter::wifi_event(WiFiEvent_t event) {
  //   switch(event) {
  //     case SYSTEM_EVENT_STA_GOT_IP:
  //       DoLog.info("SMART - WiFi connected with IP Address: ");
  //       DoLog.info(WiFi.localIP());
  //       break;
  //     case SYSTEM_EVENT_STA_DISCONNECTED:
  //       DoLog.info("SMART - WiFi lost connection");
  //       break;
  //     default: break;
  //   }
  // }

  // Let's just use this one for logging
  // WARNING - Serial is probable thread-safe.
  // void SmartDigitalMeter::on_mqtt_event(DatagramPublisher::MqttEvent event) {
  //   if (event == DatagramPublisher::MqttEvent::CONNECTED) {
  //     DoLog.info("SMART - Detected MQTT connection.");
  //   } else if (event == DatagramPublisher::MqttEvent::DISCONNECTED) {
  //     DoLog.warning("SMART - Lost the MQTT connection.");
  //   }
  // }

  // Async MQTT Client has some problems when WiFi connection is not operational
  // yet or disconnects. To solve most problems we create and destroy the publisher
  // on WiFi connect and disconnect
  void SmartDigitalMeter::communications_check(void) {
    if (WiFi.status() != WL_CONNECTED) {
      if (commState != CommState::AWAITING_WIFI) {
        commState = CommState::WIFI_LOST;
        wifiTimeoutCounter = 0;
      }
    }

    switch(commState) {
      case CommState::AWAITING_WIFI:
        DoLog.verbose("Awaiting WiFi", "comm-state");
        deviceStatus->no_communication();
        if (WiFi.status() == WL_CONNECTED) {
          commState = CommState::SETUP_MQTT;
          return;
        }
        wifiTimeoutCounter++;
        if (wifiTimeoutCounter >= 10) {
          DoLog.warning("Can not connect to WiFi.", "comm-state");
          wifiTimeoutCounter = 0;
        }
        break;

      case CommState::SETUP_MQTT:
        deviceStatus->wifi_no_mqtt();
        DoLog.verbose("Awaiting MQTT", "comm-state");
        commState = CommState::AWAITING_MQTT;
        mqttTimeoutCounter = 0;
        setup_publisher();
        break;

      case CommState::AWAITING_MQTT:
        if (publisher) {
          if (publisher->connected()) {
            commState = CommState::ALL_OPERATIONAL;
            DoLog.info("All operational", "comm-state");
          } else {
            DoLog.verbose("Publisher created but not connected", "comm-state");
            mqttTimeoutCounter++;
            if (mqttTimeoutCounter >= 6) {
              DoLog.warning("Could not connect publisher to mqtt broker", "comm-state");
              commState = CommState::SETUP_MQTT;
            }
          }
        } else {
          commState = CommState::SETUP_MQTT;
        }
        break;

      case CommState::ALL_OPERATIONAL:
        DoLog.verbose("All operational", "comm-state");
        deviceStatus->wifi_and_mqtt_ok();

        if (!publisher || !publisher->connected()) {
          DoLog.warning("Lost connection to MQTT broker", "comm-state");
          commState = CommState::SETUP_MQTT;
        }
        break;

      case CommState::WIFI_LOST:
        DoLog.warning("Lost WiFi", "comm-state");
        if (publisher) destroy_publisher();
        commState = CommState::AWAITING_WIFI;
        break;
    }
  }

  void SmartDigitalMeter::setup_publisher(void) {
    if (publisher) destroy_publisher();

    DoLog.verbose("Creating new MQTT publisher", "smart");
    publisher = new DatagramPublisher();
    if (!publisher) {
      DoLog.error("Serious fail. Could not create MQTT publisher", "smart");
      // Panic here ?
      return;
    }
    // publisher->on_mqtt_event(std::bind(&SmartDigitalMeter::on_mqtt_event, this, std::placeholders::_1));
    publisher->connect(deviceConfig->mqtt_broker(), deviceConfig->mqtt_port());
  }

  void SmartDigitalMeter::destroy_publisher(void) {
    if (publisher) {
      DoLog.verbose("Destroying MQTT publisher", "smart");
      // publisher->on_mqtt_event(nullptr);      // Don't want the events anymore
      publisher->disconnect();
      delete publisher;
      publisher = nullptr;
    }
  }

};