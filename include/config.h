#pragma once

#include "secrets.h"

// --- Hardware ---
#define RELAY_PIN D1

// --- Relay soft-PWM defaults ---
#define DEFAULT_DUTY_CYCLE  25    // percent (0–100)
#define DEFAULT_DURATION    4.0f  // seconds (full ON+OFF cycle)
