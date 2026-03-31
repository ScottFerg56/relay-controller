#include <Adafruit_seesaw.h>
#include <seesaw_neopixel.h>
#include "encoder_ctrl.h"
#include "relay_ctrl.h"
#include "config.h"

static Adafruit_seesaw   ssLeft;
static Adafruit_seesaw   ssRight;
static seesaw_NeoPixel   pixLeft (1, ENCODER_NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
static seesaw_NeoPixel   pixRight(1, ENCODER_NEOPIX_PIN, NEO_GRB + NEO_KHZ800);

static int32_t lastLeftPos  = 0;
static int32_t lastRightPos = 0;

static bool     leftBtnLast   = HIGH;
static bool     rightBtnLast  = HIGH;
static uint32_t leftBtnTime   = 0;
static uint32_t rightBtnTime  = 0;

static RelayState lastState = RelayState::PWM;
static bool       lastOn    = false;

static void updateNeopixels(RelayState state, bool on) {
    uint32_t color;
    if (state == RelayState::RELAY_OFF)
        color = pixLeft.Color(80, 60, 0);   // yellow
    else if (on)
        color = pixLeft.Color(0, 80, 0);    // green
    else
        color = pixLeft.Color(0, 0, 80);    // blue

    pixLeft.setPixelColor(0, color);
    pixLeft.show();
    pixRight.setPixelColor(0, color);
    pixRight.show();
}

void encoderBegin() {
    ssLeft.begin(ENCODER_LEFT_ADDR);
    ssLeft.pinMode(ENCODER_SWITCH_PIN, INPUT_PULLUP);
    ssLeft.setEncoderPosition(0);
    lastLeftPos = 0;

    pixLeft.begin(ENCODER_LEFT_ADDR);
    pixLeft.setBrightness(80);

    ssRight.begin(ENCODER_RIGHT_ADDR);
    ssRight.pinMode(ENCODER_SWITCH_PIN, INPUT_PULLUP);
    ssRight.setEncoderPosition(0);
    lastRightPos = 0;

    pixRight.begin(ENCODER_RIGHT_ADDR);
    pixRight.setBrightness(80);

    updateNeopixels(relay.getState(), relay.isOn());
}

void encoderUpdate() {
    uint32_t now = millis();

    // --- Left encoder: duty cycle ---
    int32_t leftPos = ssLeft.getEncoderPosition();
    if (leftPos != lastLeftPos) {
        int delta = (int)(lastLeftPos - leftPos);
        lastLeftPos = leftPos;
        int newDuty = constrain(relay.getDutyCycle() + delta * DUTY_STEP, 0, 100);
        relay.setParams(newDuty, relay.getDuration());
    }

    // --- Right encoder: duration ---
    int32_t rightPos = ssRight.getEncoderPosition();
    if (rightPos != lastRightPos) {
        int delta = (int)(lastRightPos - rightPos);
        lastRightPos = rightPos;
        float newDur = relay.getDuration() + delta * DURATION_STEP;
        if (newDur < 1.0f) newDur = 1.0f;
        relay.setParams(relay.getDutyCycle(), newDur);
    }

    // --- Left button: PWM/ALWAYS_ON -> DISABLED, DISABLED -> PWM ---
    bool leftBtn = ssLeft.digitalRead(ENCODER_SWITCH_PIN);
    if (leftBtn == LOW && leftBtnLast == HIGH && (now - leftBtnTime) > BUTTON_DEBOUNCE_MS) {
        leftBtnTime = now;
        if (relay.getState() == RelayState::RELAY_OFF)
            relay.enable();
        else
            relay.disable();
    }
    leftBtnLast = leftBtn;

    // --- Right button: PWM/DISABLED -> ALWAYS ON, ALWAYS ON -> PWM ---
    bool rightBtn = ssRight.digitalRead(ENCODER_SWITCH_PIN);
    if (rightBtn == LOW && rightBtnLast == HIGH && (now - rightBtnTime) > BUTTON_DEBOUNCE_MS) {
        rightBtnTime = now;
        if (relay.getState() == RelayState::RELAY_ON)
            relay.enable();
        else
            relay.setOn();
    }
    rightBtnLast = rightBtn;

    // --- Update NeoPixels if state changed ---
    RelayState state = relay.getState();
    bool on          = relay.isOn();
    if (state != lastState || on != lastOn) {
        updateNeopixels(state, on);
        lastState = state;
        lastOn    = on;
    }
}
