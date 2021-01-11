#include "digital_meter.h"
#include "crc_checker.h"
#include "../rgb/rgb_led.h"

namespace SmartMeter {

  #define Debug(...) if(this->debugSerial) this->debugSerial->print(__VA_ARGS__); 
  #define DebugLn(...) if(this->debugSerial) this->debugSerial->println(__VA_ARGS__); 

  DigitalMeter::DigitalMeter(int requestPin, HardwareSerial * serial, HardwareSerial * debugSerial)
  :dataLed(DATA_LED_R, DATA_LED_G, DATA_LED_B, 4, true) , ReadyMeterColor(Color::GREEN().dim(20)) , ErrorMeterColor(Color::RED().dim(20)) {
    this->requestPin = requestPin;
    this->serial = serial;
    this->debugSerial = debugSerial;
    pinMode(requestPin, OUTPUT);
    dataLed.clear();
  }

  void DigitalMeter::enable(void) {
    DebugLn("Enabling the digital meter data request");
    digitalWrite(requestPin, HIGH);
    readPointer = 0;
    startDetected = false;
  }

  void DigitalMeter::disable(void) {
    DebugLn("Disabling the digital meter data request");
    digitalWrite(requestPin, LOW);    
  }

  void DigitalMeter::timeout(void) {
    digitalWrite(requestPin, LOW);
    dataLed.color(ErrorMeterColor);
    SerialDebug.println("Communication with the smartmeter timed out!");        
  }

  // Read a new datagram from the P1 port
  bool DigitalMeter::read_datagram(char * buffer, size_t bufferLength) {
    if (serial->available() > 0) {
            
      // Get next byte for the P1 port
      char incomingByte = serial->read();
            
      // Look for the start of the datagram
      if (!startDetected && incomingByte == '/') {
        readPointer = 0;
        startDetected = true;
        DebugLn("Detected start of a datagram");
      }

      // Ignore all data on serial port if start was not detected
      if (startDetected) {
        buffer[readPointer++] = incomingByte;
        Debug(incomingByte);

        // Look for the end of the datagram
        if (incomingByte == '\n' && buffer[readPointer-7] == '!') {
          DebugLn("Read in full datagram");
          startDetected = false;

          DebugLn("Checking datagram CRC");
          if (CrcChecker::check_crc(buffer, readPointer)) {
            DebugLn("Datagram is valid");
            dataLed.color(ReadyMeterColor);
            return true;
          } else {
            DebugLn("Datagram is invalid - CRC Check Failed");
            return false;
          }

        } else if (readPointer >= bufferLength) {    // End of datagram not found
          DebugLn("Invalid Datagram > No end detected");
          clear_buffer(buffer, bufferLength);
          readPointer = 0;
          startDetected = false;
        }
      }
    }

    return false;
  }

  void DigitalMeter::clear_buffer(char * buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
      buffer[i] = 0;
    }
  }

};