#include "effects_manager.h"

namespace SmartMeter {

  // std::vector<Effects::Effect *> EffectsManager::effects;
  std::map<String, Effects::Effect *> EffectsManager::effects;

  void EffectsManager::start(void) {
    xTaskCreate(
      &EffectsManager::run,   /* Task function. */
      "effects",              /* String with name of task. */
      10000,                  /* Stack size in bytes. */
      NULL,                   /* Parameter passed as input of the task */
      1,                      /* Priority of the task. */
      NULL);  /* Task handle. */
  }

  void EffectsManager::clear_effects(void) {
    // Serial.println("Clearing effects");
    for (auto element : effects) {
      element.second->stop();
    }
    effects.clear();
  }

  void EffectsManager::set_effect(String identifier, Effects::Effect * effect) {
    std::map<String, Effects::Effect*>::iterator it = effects.find(identifier);
    if (it != effects.end()) {
      it->second->stop();
    }

    effects[identifier] = effect;
    effect->start();
  }

  void EffectsManager::run(void * params) {
    while (true) {
      // Serial.println("Doing taskie thingies in task");
      // mutex.lock();
      for (auto element : effects) {
        // Serial.println("Outputting effect");
        element.second->output(TICK_MS);
      }
      // mutex.unlock();

      delay(TICK_MS);
    }
  }
      
};