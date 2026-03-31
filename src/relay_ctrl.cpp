#include "relay_ctrl.h"
#include "config.h"

RelayController relay;

void RelayController::begin() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    _relay_on = false;
    setParams(DEFAULT_DUTY_CYCLE, DEFAULT_DURATION);
    _cycle_start = millis();
}

void RelayController::setParams(int duty_cycle, float duration_s) {
    _duty_cycle  = constrain(duty_cycle, 0, 100);
    _duration_ms = (uint32_t)(duration_s * 1000.0f);
    _on_ms       = (uint32_t)(_duration_ms * _duty_cycle / 100);
    _cycle_start = millis();
}

void RelayController::enable() {
    _enabled = true;
    _cycle_start = millis();
}

void RelayController::disable() {
    _enabled = false;
    _relay_on = false;
    digitalWrite(RELAY_PIN, LOW);
}

void RelayController::update() {
    if (!_enabled) return;

    uint32_t elapsed = millis() - _cycle_start;

    if (elapsed >= _duration_ms) {
        _cycle_start = millis();
        elapsed = 0;
    }

    bool should_be_on = (elapsed < _on_ms);
    if (should_be_on != _relay_on) {
        _relay_on = should_be_on;
        digitalWrite(RELAY_PIN, _relay_on ? HIGH : LOW);
    }
}
