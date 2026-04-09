#include "DisplayUI.h"
#include <LovyanGFX.hpp>

extern LGFX lcd;

DisplayUI::DisplayUI(PresetManager* presets, FrequencyEngine* engine)
    : _presets(presets)
    , _engine(engine)
    , _currentScreen(0)
    , _selectedCategory(0)
    , _selectedPreset(0)
    , _scrollOffset(0) {
}

void DisplayUI::update() {
}

void DisplayUI::drawHome() {
    lcd.fillScreen(COLOR_BG);
    lcd.setTextColor(COLOR_TEXT);
    lcd.setFont(&fonts::Font4);
    lcd.drawString("ESP32 RIFE", 10, 10);
    
    lcd.setFont(&fonts::Font2);
    lcd.setTextColor(COLOR_DIM);
    lcd.drawString("Select a category", 10, 50);
}

void DisplayUI::drawCategorySelect() {
    lcd.fillScreen(COLOR_BG);
    
    auto categories = _presets->getCategories();
    
    lcd.setTextColor(COLOR_TEXT);
    lcd.setFont(&fonts::Font4);
    lcd.drawString("Categories", 10, 10);
    
    lcd.setFont(&fonts::Font2);
    for (size_t i = 0; i < categories.size() && i < 8; i++) {
        if (i == _selectedCategory) {
            lcd.fillRect(0, 40 + i * 35, DISPLAY_WIDTH, 35, COLOR_COOPER);
            lcd.setTextColor(COLOR_BG);
        } else {
            lcd.setTextColor(COLOR_TEXT);
        }
        lcd.drawString(categories[i].c_str(), 10, 45 + i * 35);
    }
}

void DisplayUI::drawPresetList(const String& category) {
    lcd.fillScreen(COLOR_BG);
    
    auto presetsInCat = _presets->getPresetsByCategory(category);
    
    lcd.setTextColor(COLOR_COOPER);
    lcd.setFont(&fonts::Font4);
    lcd.drawString(category, 10, 10);
    
    lcd.setTextColor(COLOR_TEXT);
    lcd.setFont(&fonts::Font2);
    int visible = 7;
    for (int i = _scrollOffset; i < _scrollOffset + visible && i < presetsInCat.size(); i++) {
        if (i == _selectedPreset) {
            lcd.fillRect(0, 40 + (i - _scrollOffset) * 35, DISPLAY_WIDTH, 35, COLOR_COOPER);
            lcd.setTextColor(COLOR_BG);
        } else {
            lcd.setTextColor(COLOR_TEXT);
        }
        String name = presetsInCat[i]->name;
        if (name.length() > 45) name = name.substring(0, 42) + "...";
        lcd.drawString(name.c_str(), 10, 45 + (i - _scrollOffset) * 35);
    }
}

void DisplayUI::drawPresetDetail(Preset* preset) {
    if (!preset) return;
    
    lcd.fillScreen(COLOR_BG);
    
    lcd.setTextColor(COLOR_COOPER);
    lcd.setFont(&fonts::Font4);
    lcd.drawString(preset->name.c_str(), 10, 10);
    
    lcd.setTextColor(COLOR_TEXT);
    lcd.setFont(&fonts::Font2);
    lcd.drawString("Programs:", 10, 60);
    
    for (size_t i = 0; i < preset->programs.size() && i < 6; i++) {
        lcd.printf("  %s: %d freqs\n", 
            preset->programs[i].id.c_str(), 
            preset->programs[i].frequencies.size());
    }
}

void DisplayUI::drawPlayer() {
    lcd.fillScreen(COLOR_BG);
    
    lcd.setTextColor(COLOR_GREEN);
    lcd.setFont(&fonts::Font4);
    
    if (_engine->isPlaying()) {
        lcd.drawString("PLAYING", 10, 10);
    } else {
        lcd.drawString("PAUSED", 10, 10);
    }
    
    lcd.setTextColor(COLOR_CYAN);
    lcd.setFont(&fonts::Font6);
    char buf[32];
    sprintf(buf, "%.1f Hz", _engine->getCurrentFrequency());
    lcd.drawString(buf, 10, 80);
    
    lcd.setTextColor(COLOR_TEXT);
    lcd.setFont(&fonts::Font2);
    
    unsigned long elapsed = _engine->getElapsedTime() / 1000;
    unsigned long duration = _engine->getDuration() / 1000;
    sprintf(buf, "%02lu:%02lu / %02lu:%02lu",
        elapsed / 60, elapsed % 60,
        duration / 60, duration % 60);
    lcd.drawString(buf, 10, 150);
    
    int barWidth = DISPLAY_WIDTH - 40;
    int filledWidth = (elapsed * barWidth) / (duration > 0 ? duration : 1);
    lcd.fillRect(20, 180, barWidth, 10, COLOR_DIM);
    lcd.fillRect(20, 180, filledWidth, 10, COLOR_GREEN);
}
