/*
 * VIVES college Research
 *
 * Autor Ronny Mees & Nico De Witte
 *
 * Hardware settings for P1 firmware
 *
*/
#pragma once

// Baud rate for both hardware and software serial
#define SERIAL_DEBUG_BAUDRATE 115200
#define METER_BAUDRATE 115200

// Set DataRequest pin
#define REQUEST_PIN 21
#define BOOT_PIN A5

// RGB led pins
#define DATA_LED_R 5    // SCK
#define DATA_LED_G 18   // MOSI
#define DATA_LED_B 19   // MISO

#define COMM_LED_R 27
#define COMM_LED_G 32
#define COMM_LED_B 33

// Define a serial for debugging and for reading the meter
#define SerialDebug Serial
#define SerialMeter Serial1
