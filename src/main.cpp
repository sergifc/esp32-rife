#include <Arduino.h>
#include <SPIFFS.h>
#include <JC3248W535.h>

JC3248W535_Display display;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Testing JC3248W535 Display...");
    
    if (!display.begin()) {
        Serial.println("Display init failed!");
        while(1);
    }
    
    Serial.println("Display initialized!");
    
    auto gfx = display.getCanvas();
    
    gfx->fillScreen(RED);
    delay(500);
    gfx->fillScreen(GREEN);
    delay(500);
    gfx->fillScreen(BLUE);
    delay(500);
    gfx->fillScreen(WHITE);
    
    gfx->setTextColor(BLACK);
    gfx->setTextSize(2);
    gfx->setCursor(50, 200);
    gfx->print("Test Complete!");
    
    display.flush();
    
    Serial.println("Test complete!");
}

void loop() {
    delay(1000);
}
