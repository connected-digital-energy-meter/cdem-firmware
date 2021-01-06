#pragma once

#include "../digital_meter/datagram.h"
#include "../digital_meter/digital_meter.h"
#include "../mqtt/datagram_publisher.h"
#include "../config/configuration.h"

namespace SmartMeter {

  class SmartDigitalMeter {

    public:
      SmartDigitalMeter(void);

    public:
      void start(Configuration * config);
      void stop(void);
      void process(void);   // Call this in loop()

    private:
      // Define a program state class
      enum class State {
        IDLE,
        READING_DATAGRAM,
        DATAGRAM_READY,
        PROCESSING_DATA_GRAM,
        DATAGRAM_DECODED
      };

    private:
      char datagramBuffer[1024] = {0};
      Datagram datagram;
      DigitalMeter meter;
      DatagramPublisher publisher;
      bool isAcquiringData = false;

      // Set for periodic measurement
      long period = 10000L;      // TODO - Make config option
      unsigned long startMillis;            
      unsigned long currentMillis;
      unsigned long timeout;

      // Declare State and set state to IDLE
      State currentState = State::IDLE;
  };

};