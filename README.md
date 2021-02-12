# Fluvius Smart Meter Reader

Only tested with a [Feather Huzzah32](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather) with an ESP32-WROOM-32 chip.

## Connections

RX and TX are the additional `Serial1` pins, and are not connected to the USB/Serial converter. That means you can use them to connect to UART-devices like GPS's, fingerprint sensors, etc.

## Installing Arduino

See [https://www.arduino.cc/en/Guide](https://www.arduino.cc/en/Guide).

## Dependencies

## Arduino core for the ESP32

See [https://github.com/espressif/arduino-esp32#using-through-arduino-ide](https://github.com/espressif/arduino-esp32#using-through-arduino-ide).

Install `esp32 by Espressif Systems`. Currently tested with `v1.0.4`.

Open Boards Manager from `Tools => Board` menu and install `esp32 by Espressif Systems` platform. Currently tested with `v1.0.4`.

Next select `Adafruit ESP32 Feather` board from `Tools => Board => ESP32 Arduino` menu after installation

### Async MQTT Client

Download the library as a zip file and include it as such.

Or clone the repo in the directory `Arduino/libraries` which can be found under your user profile.

More info at [https://github.com/marvinroger/async-mqtt-client](https://github.com/marvinroger/async-mqtt-client)

This library has the following dependency: [https://github.com/me-no-dev/AsyncTCP](https://github.com/me-no-dev/AsyncTCP). Install it the same way.

### Arduino JSON

https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

Search for `ArduinoJson` in library (by Benoit Blanchon) and install `v6.17.2`.

## Problems

(Linux) Python not found in path but 3.x is installed. Just set python3 as alternative:

```bash
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.6 1
```

## Development

### Backtrace decoding

[https://github.com/me-no-dev/EspExceptionDecoder](https://github.com/me-no-dev/EspExceptionDecoder)
