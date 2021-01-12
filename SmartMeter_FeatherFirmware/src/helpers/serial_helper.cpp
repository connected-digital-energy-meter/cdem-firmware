#include "serial_helper.h"

namespace SmartMeter {

  // Blocking
  String SerialHelper::read_line(HardwareSerial * serial) {
    String output = "";
    char input;

    do {
      if (serial->available()) {
        input = serial->read();
        if (input != '\n') {
          output += input;
        }
      }
    } while (input != '\n');

    return output;
  }

};