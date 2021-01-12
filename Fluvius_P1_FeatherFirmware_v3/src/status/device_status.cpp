#include "device_status.h"
#include "../../hardware.h"
#include "../rgb/effects/color_effect.h"
#include "../rgb/effects/blink_effect.h"
#include "../rgb/effects_manager.h"

namespace SmartMeter {

  DeviceStatus::DeviceStatus(void)
    : commLed(COMM_LED_R, COMM_LED_G, COMM_LED_B, 0, true),
      dataLed(DATA_LED_R, DATA_LED_G, DATA_LED_B, 3, true) {

    commLed.clear();  // Only allowed here ! From then on EffectsManager
    dataLed.clear();  // should have exclusive access to the LED's
    EffectsManager::start();
  }

  void DeviceStatus::clear(void) {
    EffectsManager::clear_effects();
  }

  void DeviceStatus::booting(void) {
    static Effects::BlinkEffect commEffect(&commLed, Color::BLUE().dim(LED_BRIGHTNESS), 1000);
    static Effects::BlinkEffect dataEffect(&dataLed, Color::BLUE().dim(LED_BRIGHTNESS), 1000);

    set_duo_effect(&commEffect, &dataEffect);
  }

  void DeviceStatus::boot_menu(void) {
    static Effects::BlinkEffect commEffect(&commLed, Color::BLUE().dim(LED_BRIGHTNESS), 500);
    static Effects::BlinkEffect dataEffect(&dataLed, Color::BLUE().dim(LED_BRIGHTNESS), 500, 500);

    set_duo_effect(&commEffect, &dataEffect);
  }

  void DeviceStatus::config_wizard(void) {
    static Effects::BlinkEffect commEffect(&commLed, Color::BLUE().dim(LED_BRIGHTNESS), 100);
    static Effects::BlinkEffect dataEffect(&dataLed, Color::BLUE().dim(LED_BRIGHTNESS), 100);

    set_duo_effect(&commEffect, &dataEffect);
  }

  void DeviceStatus::done_booting(void) {
    clear();
  }

  void DeviceStatus::no_communication(void) {
    wifiOk = false;
    mqttOk = false;
    set_communication_led();
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
    static Effects::ColorEffect allOk(&commLed, Color::GREEN().dim(LED_BRIGHTNESS));
    static Effects::ColorEffect wifiOkNoMqtt(&commLed, Color::PURPLE().dim(LED_BRIGHTNESS));
    static Effects::BlinkEffect noWifi(&commLed, Color::RED().dim(LED_BRIGHTNESS), 250);

    if (mqttOk && wifiOk) EffectsManager::set_effect("comm", &allOk);
    else if (!mqttOk && wifiOk) EffectsManager::set_effect("comm", &wifiOkNoMqtt);
    else EffectsManager::set_effect("comm", &noWifi);
  }

  void DeviceStatus::meter_starting(void) {
    static Effects::ColorEffect starting(&dataLed, Color::PURPLE().dim(LED_BRIGHTNESS));

    EffectsManager::set_effect("data", &starting);
  }

  void DeviceStatus::meter_data_ready(void) {
    static Effects::ColorEffect ok(&dataLed, Color::GREEN().dim(LED_BRIGHTNESS));

    EffectsManager::set_effect("data", &ok);
  }

  void DeviceStatus::meter_error(void) {
    static Effects::BlinkEffect dataError(&dataLed, Color::RED().dim(LED_BRIGHTNESS), 250);

    EffectsManager::set_effect("data", &dataError);
  }

  void DeviceStatus::set_duo_effect(Effects::Effect * commEffect, Effects::Effect * dataEffect) {
    clear();
    EffectsManager::set_effect("comm", commEffect);
    EffectsManager::set_effect("data", dataEffect);
  }
  
};
