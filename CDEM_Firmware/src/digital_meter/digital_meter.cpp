#include "digital_meter.h"
#include "crc_checker.h"
#include "../logging/logger.h"
namespace SmartMeter {

  DigitalMeter::DigitalMeter(int requestPin, DeviceStatus * deviceStatus, HardwareSerial * serial) {
    this->deviceStatus = deviceStatus;
    this->requestPin = requestPin;
    this->serial = serial;
    pinMode(requestPin, OUTPUT);
  }

  void DigitalMeter::enable(void) {
    DoLog.verbose("Enabling the digital meter data request", "digital-meter");
    digitalWrite(requestPin, HIGH);
    readPointer = 0;
    startDetected = false;
  }

  void DigitalMeter::disable(void) {
    DoLog.verbose("Disabling the digital meter data request", "digital-meter");
    digitalWrite(requestPin, LOW);    
  }

  void DigitalMeter::timeout(void) {
    digitalWrite(requestPin, LOW);
    deviceStatus->meter_error();
    DoLog.warning("Communication with the smartmeter timed out", "digital-meter");
  }

  // Read a new datagram from the P1 port
  bool DigitalMeter::read_datagram(char * buffer, size_t bufferLength) {
    if (serial->available() > 0) {
            
      // Get next byte for the P1 port
      char incomingByte = serial->read();
            
      // Look for the start of the datagram
      if (incomingByte == '/') {
        readPointer = 0;
        startDetected = true;
        DoLog.verbose("Detected start of a datagram", "digital-meter");
        clear_buffer(buffer, bufferLength);
      }

      // Ignore all data on serial port if start was not detected
      if (startDetected) {
        buffer[readPointer++] = incomingByte;
        // Debug(incomingByte);

        // Look for the end of the datagram
        if (incomingByte == '\n' && buffer[readPointer-7] == '!') {
          DoLog.verbose("Read in full datagram", "digital-meter");
          startDetected = false;
          buffer[readPointer] = '\0';   // Null-terminate buffer

          DoLog.verbose(String(buffer), "digital-meter");

          if (CrcChecker::check_crc(buffer, readPointer)) {
            DoLog.verbose("Datagram is valid (CRC check passed)", "digital-meter");
            deviceStatus->meter_data_ready();
            return true;
          } else {
            DoLog.warning("Datagram is invalid (CRC check failed)", "digital-meter");
            return false;
          }

        } else if (readPointer >= bufferLength) {    // End of datagram not found
          DoLog.warning("Datagram is invalid (could not detect END)", "digital-meter");
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