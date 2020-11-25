# Fluvius Smart Meter Reader

Only tested with a [Feather Huzzah32](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather) with an ESP32-WROOM-32 chip.

## Connections

RX and TX are the additional `Serial1` pins, and are not connected to the USB/Serial converter. That means you can use them to connect to UART-devices like GPS's, fingerprint sensors, etc.
