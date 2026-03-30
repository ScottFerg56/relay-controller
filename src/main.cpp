#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "relay_ctrl.h"

void wifiServerBegin();
void wifiServerHandle();

void setup() {
    Serial.begin(115200);
    delay(500);

    relay.begin();

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
    relay.update();
}
