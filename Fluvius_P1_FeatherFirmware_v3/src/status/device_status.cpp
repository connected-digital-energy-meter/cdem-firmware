#include "device_status.h"
#include "../../hardware.h"

namespace SmartMeter {

  DeviceStatus::DeviceStatus(void)
    : commLed(COMM_LED_R, COMM_LED_G, COMM_LED_B, 0, true),
      dataLed(DATA_LED_R, DATA_LED_G, DATA_LED_B, 3, true) {

    clear();
  }

  void DeviceStatus::clear(void) {
    commLed.clear();
    dataLed.clear();
  }

  void DeviceStatus::booting(void) {
    commLed.color(Color::BLUE().dim(LED_BRIGHTNESS));
    dataLed.color(Color::BLUE().dim(LED_BRIGHTNESS));
  }

  void DeviceStatus::boot_menu(void) {
    commLed.color(Color::PURPLE().dim(LED_BRIGHTNESS));
    dataLed.color(Color::PURPLE().dim(LED_BRIGHTNESS));
  }

  void DeviceStatus::config_wizard(void) {
    commLed.color(Color::YELLOW().dim(LED_BRIGHTNESS));
    dataLed.color(Color::YELLOW().dim(LED_BRIGHTNESS));
  }

  void DeviceStatus::no_communication(void) {
    wifiOk = false;
    mqttOk = false;
  }

  void DeviceStatus::wifi_ok(void) {
    wifiOk = true;
    set_communication_led();
  }

  void DeviceStatus::wifi_error(void) {
    wifiOk = false;
    set_communication_led();
  }


  void DeviceStatus::mqtt_ok(void) {
    mqttOk = true;
    set_communication_led();
  }

  void DeviceStatus::mqtt_error(void) {
    mqttOk = false;
    set_communication_led();
  }

  void DeviceStatus::set_communication_led(void) {
    if (mqttOk && wifiOk) commLed.color(Color::GREEN().dim(LED_BRIGHTNESS));
    else if (!mqttOk && wifiOk) commLed.color(Color::YELLOW().dim(LED_BRIGHTNESS));
    else if (!mqttOk && !wifiOk) commLed.color(Color::RED().dim(LED_BRIGHTNESS));
    else commLed.color(Color::BLUE().dim(LED_BRIGHTNESS));
  }

  void DeviceStatus::meter_awaiting(void) {
    dataLed.color(Color::YELLOW().dim(LED_BRIGHTNESS));
  }

  void DeviceStatus::meter_data_ready(void) {
    dataLed.color(Color::GREEN().dim(LED_BRIGHTNESS));
  }

  void DeviceStatus::meter_error(void) {
    dataLed.color(Color::RED().dim(LED_BRIGHTNESS));
  }
  
};
