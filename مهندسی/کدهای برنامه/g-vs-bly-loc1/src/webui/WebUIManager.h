#pragma once
/*
------------------------------------------------------------
GlassGarden — Web UI Manager

وظیفه:
- راه‌اندازی سرور وب محلی روی ESP32
- ارتباط real-time با WebSocket
- کنترل تجهیزات و نمایش سنسورها بدون وابستگی به Blynk

Version : 1.0.0
------------------------------------------------------------
*/

/*
------------------------------------------------------------
GlassGarden — Web UI Manager

Version : 1.1.0
------------------------------------------------------------
*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "Images.h"

class WebUIManager {
public:
    void begin();
    void update();
    void broadcastState();

private:
    AsyncWebServer server{80};
    AsyncWebSocket ws{"/ws"};
    portMUX_TYPE stateMux = portMUX_INITIALIZER_UNLOCKED;

    void setupRoutes();
    void onWsEvent(AsyncWebSocketClient* client, AwsEventType type,
                   void* arg, uint8_t* data, size_t len);
    void handleApiControl(AsyncWebServerRequest* request, uint8_t* data, size_t len);

    String getIndexedHtml();
};

extern WebUIManager webUI;