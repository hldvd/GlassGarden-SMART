#include "WebUIManager.h"
#include "WebUICss.h"
#include "WebUIHtml.h"
#include "../core/Config.h"
#include "../state/StateManager.h"
#include "../devices/DeviceManager.h"
#include "../automation/AutomationManager.h"

WebUIManager webUI;

void WebUIManager::begin()
{
    setupRoutes();
    
    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, 
                      AwsEventType type, void* arg, uint8_t* data, size_t len) {
        this->onWsEvent(client, type, arg, data, len);
    });
    
    server.addHandler(&ws);
    server.begin();
    Serial.println("[WebUI] Server started on http://" + WiFi.localIP().toString());
}

String WebUIManager::getIndexedHtml()
{
    String html = FPSTR(WEBUI_HTML);
    String css  = FPSTR(WEBUI_CSS);
    
    html.replace("{{CSS}}", css);
    
    html.replace("var(--img-logo)",   String("url(\"") + FPSTR(CSS_LOGO_URL)   + "\")");
    html.replace("var(--img-nature)", String("url(\"") + FPSTR(CSS_NATURE_URL) + "\")");
    
    return html;
}

void WebUIManager::setupRoutes()
{
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request){
        String html = this->getIndexedHtml();
        request->send(200, "text/html", html);
    });

    server.on("/api/control", HTTP_POST,
        [](AsyncWebServerRequest* request){},
        NULL,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total){
            handleApiControl(request, data, len);
        });

    server.on("/api/state", HTTP_GET, [this](AsyncWebServerRequest* request){
        JsonDocument doc;
        
        portENTER_CRITICAL(&this->stateMux);
        doc["temperature"] = state.temperature;
        doc["humidity"] = state.humidity;
        doc["waterLevelPercent"] = state.waterLevelPercent;
        doc["waterEmpty"] = state.waterEmpty;
        doc["light"] = state.light;
        doc["fan"] = state.fan;
        doc["fogger"] = state.fogger;
        doc["pump"] = state.pump;
        doc["autoMode"] = state.autoMode;
        doc["safeMode"] = state.safeMode;
        doc["wifiConnected"] = state.wifiConnected;
        doc["blynkConnected"] = state.blynkConnected;
        portEXIT_CRITICAL(&this->stateMux);

        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });
}

void WebUIManager::onWsEvent(AsyncWebSocketClient* client, AwsEventType type,
                             void* arg, uint8_t* data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("[WebUI] Client #%u connected from %s\n", 
                      client->id(), client->remoteIP().toString().c_str());
        broadcastState();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("[WebUI] Client #%u disconnected\n", client->id());
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo* info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            // Fix 1: Safe copy — no buffer overflow
            String msg;
            msg.reserve(len);
            for (size_t i = 0; i < len; i++) {
                msg += (char)data[i];
            }
            
            JsonDocument doc;
            DeserializationError err = deserializeJson(doc, msg.c_str());
            if (err) return;

            // Fix 3: Null-pointer guard
            const char* msgType = doc["type"];
            if (!msgType) return;
            
            if (strcmp(msgType, "cmd") == 0)
            {
                const char* device = doc["device"];
                if (!device) return;
                
                bool value = doc["value"] | false;
                
                portENTER_CRITICAL(&this->stateMux);
                bool inAuto = state.autoMode;
                portEXIT_CRITICAL(&this->stateMux);
                
                if (!inAuto)
                {
                    portENTER_CRITICAL(&this->stateMux);
                    if (strcmp(device, "light") == 0)       value ? devices.lightOn() : devices.lightOff();
                    else if (strcmp(device, "fan") == 0)    value ? devices.fanOn() : devices.fanOff();
                    else if (strcmp(device, "fogger") == 0) value ? devices.foggerOn() : devices.foggerOff();
                    else if (strcmp(device, "pump") == 0)   value ? devices.pumpOn() : devices.pumpOff();
                    portEXIT_CRITICAL(&this->stateMux);
                }
                else
                {
                    Serial.printf("[WebUI] Ignored manual cmd in AUTO mode: %s=%d\n", device, value);
                }
                broadcastState();
            }
            else if (strcmp(msgType, "mode") == 0)
            {
                portENTER_CRITICAL(&this->stateMux);
                state.autoMode = doc["value"] | false;
                bool newMode = state.autoMode;
                portEXIT_CRITICAL(&this->stateMux);
                Serial.printf("[WebUI] Mode changed to: %s\n", newMode ? "AUTO" : "MANUAL");
                broadcastState();
            }
        }
    }
}

void WebUIManager::broadcastState()
{
    JsonDocument doc;
    doc["type"] = "state";
    JsonObject pl = doc["payload"].to<JsonObject>();
    
    portENTER_CRITICAL(&this->stateMux);
    pl["temperature"] = state.temperature;
    pl["humidity"] = state.humidity;
    pl["waterLevelPercent"] = state.waterLevelPercent;
    pl["waterEmpty"] = state.waterEmpty;
    pl["light"] = state.light;
    pl["fan"] = state.fan;
    pl["fogger"] = state.fogger;
    pl["pump"] = state.pump;
    pl["autoMode"] = state.autoMode;
    pl["safeMode"] = state.safeMode;
    portEXIT_CRITICAL(&this->stateMux);

    String payload;
    serializeJson(doc, payload);
    ws.textAll(payload);
}

void WebUIManager::handleApiControl(AsyncWebServerRequest* request, uint8_t* data, size_t len)
{
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, data, len);
    if (err)
    {
        request->send(400, "application/json", "{\"error\":\"invalid json\"}");
        return;
    }
    
    const char* device = doc["device"];
    if (!device)
    {
        request->send(400, "application/json", "{\"ok\":false,\"error\":\"missing device\"}");
        return;
    }
    
    bool value = doc["value"] | false;
    bool executed = false;
    
    portENTER_CRITICAL(&this->stateMux);
    bool inAuto = state.autoMode;
    portEXIT_CRITICAL(&this->stateMux);
    
    if (!inAuto)
    {
        portENTER_CRITICAL(&this->stateMux);
        if (strcmp(device, "light") == 0)       { value ? devices.lightOn() : devices.lightOff(); executed = true; }
        else if (strcmp(device, "fan") == 0)    { value ? devices.fanOn() : devices.fanOff(); executed = true; }
        else if (strcmp(device, "fogger") == 0) { value ? devices.foggerOn() : devices.foggerOff(); executed = true; }
        else if (strcmp(device, "pump") == 0)   { value ? devices.pumpOn() : devices.pumpOff(); executed = true; }
        portEXIT_CRITICAL(&this->stateMux);
    }
    
    broadcastState();
    
    if (executed)
        request->send(200, "application/json", "{\"ok\":true}");
    else
        request->send(200, "application/json", "{\"ok\":false,\"reason\":\"auto_mode_or_unknown_device\"}");
}

void WebUIManager::update()
{
    ws.cleanupClients();
    
    static unsigned long lastBroadcast = 0;
    if (millis() - lastBroadcast >= 2000)
    {
        lastBroadcast = millis();
        if (ws.count() > 0)
        {
            broadcastState();
        }
    }
}