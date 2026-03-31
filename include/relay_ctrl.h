#pragma once
#include <Arduino.h>

enum class RelayState { PWM, RELAY_OFF, RELAY_ON };

class RelayController {
public:
    void begin();
    void setParams(int duty_cycle, float duration_s);
    void update();
    void enable();
    void disable();
    void setOn();

    RelayState getState()      const { return _state; }
    bool  isEnabled()          const { return _state != RelayState::RELAY_OFF; }
    bool  isOn()               const { return _relay_on; }
    int   getDutyCycle()       const { return _duty_cycle; }
    float getDuration()        const { return _duration_ms / 1000.0f; }

private:
    int        _duty_cycle  = 50;
    uint32_t   _duration_ms = 10000;
    uint32_t   _on_ms       = 5000;
    uint32_t   _cycle_start = 0;
    bool       _relay_on    = false;
    RelayState _state       = RelayState::PWM;
};

extern RelayController relay;
