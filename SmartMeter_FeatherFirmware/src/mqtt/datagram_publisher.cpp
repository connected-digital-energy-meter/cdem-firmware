#include "datagram_publisher.h"

namespace SmartMeter {
  #define Debug(...) if(this->debugSerial) this->debugSerial->print(__VA_ARGS__); 
  #define DebugLn(...) if(this->debugSerial) this->debugSerial->println(__VA_ARGS__); 

  DatagramPublisher::DatagramPublisher(HardwareSerial * debugSerial) {
    this->debugSerial = debugSerial;
    setup_callbacks();
  }

  DatagramPublisher::~DatagramPublisher(void) {
    mqttClient.disconnect(true);
  }

  void DatagramPublisher::connect(String host, uint16_t port) {
    if (mqttClient.connected()) {
      DebugLn("MQTT - Already connected. Disconnecting ...");
      mqttClient.disconnect();
    }

    mqttClient.setServer(host.c_str(), port);
    _connect();
  }

  void DatagramPublisher::disconnect(void) {
    DebugLn("MQTT - Disconnecting from MQTT broker");
    mqttClient.disconnect();
  }

  bool DatagramPublisher::connected(void) {
    return mqttClient.connected();
  }

  void DatagramPublisher::on_mqtt_event(std::function<void(MqttEvent)> mqttEventCallback) {
    this->mqttEventCallback = mqttEventCallback;
  }

  void DatagramPublisher::setup_callbacks(void) {
    // std::function<void(bool sessionPresent)> OnConnectUserCallback;
    mqttClient.onConnect(std::bind(&DatagramPublisher::on_connected, this, std::placeholders::_1));

    // typedef std::function<void(AsyncMqttClientDisconnectReason reason)> OnDisconnectUserCallback;
    mqttClient.onDisconnect(std::bind(&DatagramPublisher::on_disconnected, this, std::placeholders::_1));

    // typedef std::function<void(uint16_t packetId)> OnPublishUserCallback;
    // mqttClient.onPublish(std::bind(&DatagramPublisher::on_published, this, std::placeholders::_1));
  }

  void DatagramPublisher::_connect(void) {
    if (mqttClient.connected()) {
      DebugLn("MQTT - Already operational ... doing nothing more");
      return;
    }

    DebugLn("MQTT - Connecting to MQTT broker");
    mqttClient.connect();
  }

  void DatagramPublisher::on_connected(bool sessionPresent) {
    DebugLn("MQTT - Connected to MQTT broker");
    if (mqttEventCallback) mqttEventCallback(MqttEvent::CONNECTED);
  }

  void DatagramPublisher::on_disconnected(AsyncMqttClientDisconnectReason reason) {
    DebugLn("MQTT - Disconnected from MQTT broker - Reason " + String((uint8_t)reason));
    if (mqttEventCallback) mqttEventCallback(MqttEvent::DISCONNECTED);
  }

  void DatagramPublisher::on_published(uint16_t packetId) {
    DebugLn("MQTT - Successfully published datagram with packetId = " + String(packetId));
  }

  bool DatagramPublisher::publish(Datagram * datagram, String baseTopic) {
    if (!connected()) {
      DebugLn("MQTT - Could not publish. Not connected to broker.");
      return false;
    }

    bool schededuleOk = true;
    std::vector<String> keys = datagram->keys();
    for (String key : keys) {
      String topic = baseTopic + key;
      String data = String(datagram->get(key));
      schededuleOk = schededuleOk && mqttClient.publish(topic.c_str(), 1, true, data.c_str());
    }
    
    if (schededuleOk) {
      DebugLn("MQTT - Datagram scheduled for publish");
    } else {
      DebugLn("MQTT - Failed to schedule datagram for publish");
    }
    return schededuleOk;
  }

};