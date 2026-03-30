#include <WebServer.h>
#include "relay_ctrl.h"

WebServer server(80);

static const char PAGE[] PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>Relay Controller</title>
  <style>
    body { font-family: sans-serif; max-width: 400px; margin: 40px auto; padding: 0 16px; }
    label { display: block; margin-top: 12px; font-weight: bold; }
    input[type=number] { width: 100%; padding: 6px; font-size: 1rem; box-sizing: border-box; }
    button { margin-top: 16px; width: 100%; padding: 10px; font-size: 1rem; background: #2c7be5; color: #fff; border: none; border-radius: 4px; cursor: pointer; }
    #status { margin-top: 20px; padding: 10px; background: #f0f0f0; border-radius: 4px; }
  </style>
</head>
<body>
  <h2>Relay Controller</h2>
  <form method="POST" action="/set">
    <label>Duty Cycle (%)
      <input type="number" name="duty" min="0" max="100" value="%DUTY%">
    </label>
    <label>Duration (s)
      <input type="number" name="duration" min="1" max="3600" step="0.1" value="%DUR%">
    </label>
    <button type="submit">Apply</button>
  </form>
  <div id="status">Relay: <strong id="rstate">%STATE%</strong></div>
  <script>
    setInterval(function() {
      fetch('/status').then(r => r.json()).then(d => {
        document.getElementById('rstate').textContent = d.relay;
      });
    }, 500);
  </script>
</body>
</html>
)html";

static String buildPage() {
    String html = FPSTR(PAGE);
    html.replace("%DUTY%",  String(relay.getDutyCycle()));
    html.replace("%DUR%",   String(relay.getDuration(), 1));
    html.replace("%STATE%", relay.isOn() ? "ON" : "OFF");
    return html;
}

static void handleRoot() {
    server.send(200, "text/html", buildPage());
}

static void handleSet() {
    if (server.hasArg("duty") && server.hasArg("duration")) {
        int   duty = server.arg("duty").toInt();
        float dur  = server.arg("duration").toFloat();
        relay.setParams(duty, dur);
    }
    server.sendHeader("Location", "/");
    server.send(303);
}

static void handleStatus() {
    String json = "{\"duty_cycle\":";
    json += relay.getDutyCycle();
    json += ",\"duration\":";
    json += relay.getDuration();
    json += ",\"relay\":\"";
    json += relay.isOn() ? "ON" : "OFF";
    json += "\"}";
    server.send(200, "application/json", json);
}

void wifiServerBegin() {
    server.on("/",       HTTP_GET,  handleRoot);
    server.on("/set",    HTTP_POST, handleSet);
    server.on("/status", HTTP_GET,  handleStatus);
    server.begin();
}

void wifiServerHandle() {
    server.handleClient();
}
