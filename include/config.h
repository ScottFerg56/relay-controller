#pragma once

#include "secrets.h"

// --- Network ---
#define STATIC_IP      IPAddress(192, 168, 0, 210)
#define GATEWAY_IP     IPAddress(192, 168, 0, 1)
#define SUBNET_MASK    IPAddress(255, 255, 255, 0)

// --- Hardware ---
#define RELAY_PIN D1

// --- Relay soft-PWM defaults ---
#define DEFAULT_DUTY_CYCLE  25    // percent (0–100)
#define DEFAULT_DURATION    4.0f  // seconds (full ON+OFF cycle)

// --- Encoders ---
#define ENCODER_LEFT_ADDR   0x36
#define ENCODER_RIGHT_ADDR  0x37
#define ENCODER_SWITCH_PIN  24     // seesaw pin for encoder push button
#define DUTY_STEP           5      // % per detent
#define DURATION_STEP       0.5f   // seconds per detent
#define BUTTON_DEBOUNCE_MS  50
