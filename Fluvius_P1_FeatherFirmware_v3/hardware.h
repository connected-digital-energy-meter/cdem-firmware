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
const int REQUEST_PIN = 14;
const int BOOT_PIN = 21;        // TODO - Replace with real pin

// RGB led pins
#define DATA_LED_R 5    // SCK
#define DATA_LED_G 18   // MOSI
//#define DATA_LED_B 19   // MISO
#define DATA_LED_B 13
#define COMM_LED_R 27
#define COMM_LED_G 32
#define COMM_LED_B 33

// Define a serial for debugging and for reading the meter
#define SerialDebug Serial
#define SerialMeter Serial1
