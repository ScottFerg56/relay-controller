#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "relay_ctrl.h"
#include "encoder_ctrl.h"
#include "display_ctrl.h"

void wifiServerBegin();
void wifiServerHandle();

void setup() {
    Serial.begin(115200);
    delay(500);

    relay.begin();
    encoderBegin();
    displayBegin();

    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());

    wifiServerBegin();
    Serial.println("HTTP server started.");
}

void loop() {
    wifiServerHandle();
    encoderUpdate();
    relay.update();
    displayUpdate();
}
