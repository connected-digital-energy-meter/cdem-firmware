# Fluvius Smart Meter Reader

Only tested with a [Feather Huzzah32](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather) with an ESP32-WROOM-32 chip.

## Connections

RX and TX are the additional `Serial1` pins, and are not connected to the USB/Serial converter. That means you can use them to connect to UART-devices like GPS's, fingerprint sensors, etc.

## Dependencies

### Async MQTT Client

Download the library as a zip file and include it as such.

Or clone the repo in the directory `Arduino/libraries` which can be found under your user profile.

More info at [https://github.com/marvinroger/async-mqtt-client](https://github.com/marvinroger/async-mqtt-client)

This library has the following dependency: [https://github.com/me-no-dev/AsyncTCP](https://github.com/me-no-dev/AsyncTCP). Install it the same way.
