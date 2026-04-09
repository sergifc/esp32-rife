#include "WifiManager.h"
#include <FS.h>

WifiManager::WifiManager() : _ssid(nullptr), _password(nullptr), _reconnectAttempt(0) {}

void WifiManager::begin(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED && _reconnectAttempt < 30) {
        delay(500);
        Serial.print(".");
        _reconnectAttempt++;
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("WiFi connected. IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("WiFi connection failed");
    }
}

void WifiManager::handle() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        delay(100);
        WiFi.begin(_ssid, _password);
        _reconnectAttempt = 0;
    }
}

bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WifiManager::getIP() {
    return WiFi.localIP().toString();
}

bool WifiManager::downloadFile(const char* url, const char* path) {
    HTTPClient http;
    WiFiClient client;
    
    http.begin(client, url);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        File file = SPIFFS.open(path, FILE_WRITE);
        if (file) {
            http.writeToStream(&file);
            file.close();
            Serial.println("File downloaded successfully");
            return true;
        }
    }
    
    Serial.print("Download failed: ");
    Serial.println(httpCode);
    http.end();
    return false;
}
