#pragma once

#include "datagram.h"
#include <AsyncMqttClient.h>

namespace SmartMeter {

  class MqttService {

    public:
      static void publish(Datagram * _datagram,AsyncMqttClient * _mqttClient);


  };

};