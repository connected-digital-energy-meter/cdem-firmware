#include "smart_digital_meter.h"

#include "../digital_meter/decoder.h"
#include "../../hardware.h"

namespace SmartMeter {

  SmartDigitalMeter::SmartDigitalMeter(void)
    : meter(REQUEST_PIN, &SerialMeter, &SerialDebug),
      publisher(&SerialDebug) {

    SerialMeter.begin(METER_BAUDRATE);
    pinMode(REQUEST_PIN, OUTPUT);
    meter.disable();
  }

  void SmartDigitalMeter::start(Configuration * config) {
    this->period = config->read_freq();
    publisher.connect(config->mqtt_broker(), config->mqtt_port());

    // if connected:    // TODO
    isAcquiringData = true;

    // Start time for period
    startMillis = millis();
  }

  void SmartDigitalMeter::stop(void) {
    isAcquiringData = false;
  }

  void SmartDigitalMeter::process(void) {
    // Current time for period
    currentMillis = millis();

    // Wait until next period  
    if(isAcquiringData && (currentMillis - startMillis) >= period) {

      switch (currentState){
        // Request data
        case State::IDLE:
          meter.enable();
          currentState = State::READING_DATAGRAM;
          break;
        // Read data
        case State::READING_DATAGRAM:
          if (meter.read_datagram(datagramBuffer, sizeof(datagramBuffer))) {
            currentState = State::DATAGRAM_READY;
          }
          break;
        // Stop requesting data
        case State::DATAGRAM_READY:        
          meter.disable();
          currentState = State::PROCESSING_DATA_GRAM;
          break;
        // Decode data  
        case State::PROCESSING_DATA_GRAM:
          datagram = SmartMeter::Decoder::decode(datagramBuffer, sizeof(datagramBuffer));
          SerialDebug.println("Decoded datagram:");
          SerialDebug.println(datagram.to_string());
          currentState = State::DATAGRAM_DECODED;
          break;
        // Publish data to MQTT  
        case State::DATAGRAM_DECODED:
          publisher.publish(&datagram);
          SerialDebug.println("Datagram published");
          // Ready for next request    
          currentState = State::IDLE;
          // reset timer
          startMillis = currentMillis;
          break;
      }
    }
  }

};