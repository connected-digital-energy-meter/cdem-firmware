#include "effects_manager.h"

namespace SmartMeter {

  std::map<String, Effects::Effect *> EffectsManager::effects;
  SemaphoreHandle_t EffectsManager::mutex;

  void EffectsManager::start(void) {
    xTaskCreate(
      &EffectsManager::run,   /* Task function. */
      "effects",              /* String with name of task. */
      10000,                  /* Stack size in bytes. */
      NULL,                   /* Parameter passed as input of the task */
      1,                      /* Priority of the task. */
      NULL);                  /* Task handle. */

    mutex = xSemaphoreCreateMutex();
  }

  void EffectsManager::clear_effects(void) {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      for (auto element : effects) {
        element.second->stop();
      }
      effects.clear();

      xSemaphoreGive(mutex);
    }
  }

  void EffectsManager::set_effect(String identifier, Effects::Effect * effect) {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      std::map<String, Effects::Effect*>::iterator it = effects.find(identifier);
      if (it != effects.end()) {
        it->second->stop();
      }

      effects[identifier] = effect;
      effect->start();

      xSemaphoreGive(mutex);
    }
  }

  void EffectsManager::run(void * params) {
    while (true) {
      if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        for (auto element : effects) {
          element.second->output(TICK_MS);
        }
        xSemaphoreGive(mutex);
      }
      delay(TICK_MS);
    }
  }
      
};