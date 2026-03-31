#include <Adafruit_SSD1306.h>
#include "display_ctrl.h"
#include "relay_ctrl.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1

// Yellow zone: rows 0–15  (16px tall)
// Blue zone:   rows 16–63 (48px tall)
#define BAR_X       2
#define BAR_Y       2
#define BAR_W      (SCREEN_WIDTH - 4)
#define BAR_H      (16 - 4)   // 12px tall, centered in yellow strip

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static bool     lastEnabled  = true;
static bool     lastOn       = false;
static int      lastDuty     = -1;
static float    lastDuration = -1.0f;

void displayBegin() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
}

static void drawBar(bool relayOn) {
    display.fillRect(BAR_X, BAR_Y, BAR_W, BAR_H, BLACK);
    if (relayOn) {
        display.fillRect(BAR_X, BAR_Y, BAR_W, BAR_H, WHITE);
    } else {
        display.drawRect(BAR_X, BAR_Y, BAR_W, BAR_H, WHITE);
    }
}

static void drawParams(int duty, float duration, bool enabled) {
    // Clear blue zone
    display.fillRect(0, 16, SCREEN_WIDTH, SCREEN_HEIGHT - 16, BLACK);

    display.setTextColor(WHITE);

    if (!enabled) {
        display.setTextSize(2);
        display.setCursor(22, 28);
        display.print("DISABLED");
        return;
    }

    // Duty cycle — left half, large font
    display.setTextSize(3);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d%%", duty);
    // Right-align duty in left half (0..63)
    int16_t x, y;
    uint16_t w, h;
    display.getTextBounds(buf, 0, 0, &x, &y, &w, &h);
    display.setCursor((64 - w) / 2, 22);
    display.print(buf);

    // Duration — right half, large font
    snprintf(buf, sizeof(buf), "%.1f", duration);
    display.getTextBounds(buf, 0, 0, &x, &y, &w, &h);
    display.setCursor(64 + (64 - w) / 2, 22);
    display.print(buf);
}

void displayUpdate() {
    bool on      = relay.isOn();
    bool enabled = relay.isEnabled();
    int  duty    = relay.getDutyCycle();
    float dur    = relay.getDuration();

    bool barChanged    = (on != lastOn);
    bool paramsChanged = (duty != lastDuty || dur != lastDuration || enabled != lastEnabled);

    if (!barChanged && !paramsChanged) return;

    if (barChanged) {
        drawBar(on);
        lastOn = on;
    }

    if (paramsChanged) {
        drawParams(duty, dur, enabled);
        lastDuty     = duty;
        lastDuration = dur;
        lastEnabled  = enabled;
    }

    display.display();
}
