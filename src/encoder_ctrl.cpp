#include <Adafruit_seesaw.h>
#include "encoder_ctrl.h"
#include "relay_ctrl.h"
#include "config.h"

static Adafruit_seesaw ssLeft;
static Adafruit_seesaw ssRight;

static int32_t lastLeftPos  = 0;
static int32_t lastRightPos = 0;

static bool     leftBtnLast   = HIGH;
static bool     rightBtnLast  = HIGH;
static uint32_t leftBtnTime   = 0;
static uint32_t rightBtnTime  = 0;

void encoderBegin() {
    ssLeft.begin(ENCODER_LEFT_ADDR);
    ssLeft.pinMode(ENCODER_SWITCH_PIN, INPUT_PULLUP);
    ssLeft.setEncoderPosition(0);
    lastLeftPos = 0;

    ssRight.begin(ENCODER_RIGHT_ADDR);
    ssRight.pinMode(ENCODER_SWITCH_PIN, INPUT_PULLUP);
    ssRight.setEncoderPosition(0);
    lastRightPos = 0;
}

void encoderUpdate() {
    uint32_t now = millis();

    // --- Left encoder: duty cycle ---
    int32_t leftPos = ssLeft.getEncoderPosition();
    if (leftPos != lastLeftPos) {
        int delta = (int)(leftPos - lastLeftPos);
        lastLeftPos = leftPos;
        int newDuty = constrain(relay.getDutyCycle() + delta * DUTY_STEP, 0, 100);
        relay.setParams(newDuty, relay.getDuration());
    }

    // --- Right encoder: duration ---
    int32_t rightPos = ssRight.getEncoderPosition();
    if (rightPos != lastRightPos) {
        int delta = (int)(rightPos - lastRightPos);
        lastRightPos = rightPos;
        float newDur = relay.getDuration() + delta * DURATION_STEP;
        if (newDur < 0.5f) newDur = 0.5f;
        relay.setParams(relay.getDutyCycle(), newDur);
    }

    // --- Left button: toggle enable/disable ---
    bool leftBtn = ssLeft.digitalRead(ENCODER_SWITCH_PIN);
    if (leftBtn == LOW && leftBtnLast == HIGH && (now - leftBtnTime) > BUTTON_DEBOUNCE_MS) {
        leftBtnTime = now;
        if (relay.isEnabled())
            relay.disable();
        else
            relay.enable();
    }
    leftBtnLast = leftBtn;

    // --- Right button: mode (stub) ---
    bool rightBtn = ssRight.digitalRead(ENCODER_SWITCH_PIN);
    if (rightBtn == LOW && rightBtnLast == HIGH && (now - rightBtnTime) > BUTTON_DEBOUNCE_MS) {
        rightBtnTime = now;
        // future: mode switching
    }
    rightBtnLast = rightBtn;
}
