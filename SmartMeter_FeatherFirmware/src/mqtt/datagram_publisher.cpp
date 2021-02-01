#include "datagram_publisher.h"
#include "../logging/logger.h"

namespace SmartMeter {

  DatagramPublisher::DatagramPublisher(void) {
    setup_callbacks();
  }

  DatagramPublisher::~DatagramPublisher(void) {
    mqttClient.disconnect(true);
  }

  void DatagramPublisher::connect(String host, uint16_t port) {
    if (mqttClient.connected()) {
      DoLog.info("Already connected. Disconnecting ...", "mqtt");
      mqttClient.disconnect();
    }

    mqttClient.setServer(host.c_str(), port);
    _connect();
  }

  void DatagramPublisher::disconnect(void) {
    DoLog.verbose("Disconnecting from MQTT broker ...", "mqtt");
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
      DoLog.verbose("Already operational ... doing nothing more", "mqtt");
      return;
    }

    DoLog.info("Connecting to MQTT broker", "mqtt");
    mqttClient.connect();
  }

  void DatagramPublisher::on_connected(bool sessionPresent) {
    // DebugLn("MQTT - Connected to MQTT broker");    // Not thread safe !
    if (mqttEventCallback) mqttEventCallback(MqttEvent::CONNECTED);
  }

  void DatagramPublisher::on_disconnected(AsyncMqttClientDisconnectReason reason) {
    // DebugLn("MQTT - Disconnected from MQTT broker - Reason " + String((uint8_t)reason));   // Not thread safe !
    if (mqttEventCallback) mqttEventCallback(MqttEvent::DISCONNECTED);
  }

  void DatagramPublisher::on_published(uint16_t packetId) {
    // DebugLn("MQTT - Successfully published datagram with packetId = " + String(packetId));   // Not thread safe !
  }

  bool DatagramPublisher::publish(Datagram * datagram, String baseTopic) {
    if (!connected()) {
      DoLog.warning("Could not publish. Not connected to broker.", "mqtt");
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
      DoLog.info("Datagram scheduled for publish", "mqtt");
    } else {
      DoLog.warning("Failed to schedule datagram for publish", "mqtt");
    }
    return schededuleOk;
  }

};