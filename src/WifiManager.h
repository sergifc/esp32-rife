#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class WifiManager {
public:
    WifiManager();
    
    void begin(const char* ssid, const char* password);
    void handle();
    bool isConnected();
    String getIP();
    
    bool downloadFile(const char* url, const char* path);
    
private:
    const char* _ssid;
    const char* _password;
    unsigned long _reconnectAttempt;
};
