#pragma once

#include <Arduino.h>
#include "./effects/effect.h"
#include <map>

namespace SmartMeter {

  class EffectsManager {
    // Needs to be static because of FreeRTOS task creation
    // only supports functions

    public:
      static void start(void);

    public:
      static void clear_effects(void);
      static void set_effect(String identifier, Effects::Effect * effect);

    private:
      static void run(void * params);
      
    private:
      static const unsigned int TICK_MS = 10;
      // static std::vector<Effects::Effect *> effects;
      // Mutex mutex;

      static std::map<String, Effects::Effect *> effects;
  };

};