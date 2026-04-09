#include <Arduino.h>
#include <SPIFFS.h>
#include <LovyanGFX.hpp>
#include "DisplayUI.h"
#include "FrequencyEngine.h"
#include "WifiManager.h"
#include "PresetManager.h"
#include "config.h"

LGFX lcd;
DisplayUI* ui = nullptr;
FrequencyEngine* engine = nullptr;
WifiManager* wifi = nullptr;
PresetManager* presets = nullptr;

unsigned long lastUpdate = 0;

void setupDisplay() {
    lcd.init();
    lcd.setRotation(1);
    lcd.fillScreen(0x0a0a0f);
    lcd.setFont(&fonts::Font4);
    lcd.setTextColor(TFT_WHITE);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n=== ESP32 RIFE Controller ===");
    Serial.println("Initializing...");

    setupDisplay();
    lcd.drawString("ESP32 RIFE", 10, 10);
    lcd.drawString("Loading...", 10, 50);

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        lcd.drawString("SPIFFS failed!", 10, 90);
    }

    presets = new PresetManager();
    engine = new FrequencyEngine();
    wifi = new WifiManager();
    ui = new DisplayUI(presets, engine);

    presets->loadFromSPIFFS();

    lcd.fillScreen(0x0a0a0f);
    lcd.drawString("Ready!", 10, 50);
    
    Serial.println("Initialization complete!");
}

void loop() {
    unsigned long now = millis();

    if (wifi->isConnected()) {
        wifi->handle();
    }

    engine->update();
    ui->update();

    if (now - lastUpdate > 1000) {
        lastUpdate = now;
    }

    delay(10);
}
