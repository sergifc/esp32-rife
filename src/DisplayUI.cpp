#include "DisplayUI.h"

extern Arduino_GFX *gfx;

DisplayUI::DisplayUI(PresetManager* presets, FrequencyEngine* engine)
    : _presets(presets)
    , _engine(engine)
    , _currentScreen(0)
    , _selectedCategory(0)
    , _selectedPreset(0)
    , _scrollOffset(0)
{
}

void DisplayUI::update() {
}

void DisplayUI::drawHome() {
    gfx->fillScreen(COLOR_BG);
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(2);
    gfx->drawString("ESP32 RIFE", 10, 10);
    
    gfx->setTextSize(1);
    gfx->setTextColor(COLOR_DIM);
    gfx->drawString("Select a category", 10, 50);
}

void DisplayUI::drawCategorySelect() {
    gfx->fillScreen(COLOR_BG);
    
    auto categories = _presets->getCategories();
    
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(2);
    gfx->drawString("Categories", 10, 10);
    
    gfx->setTextSize(1);
    for (size_t i = 0; i < categories.size() && i < 8; i++) {
        if (i == _selectedCategory) {
            gfx->fillRect(0, 40 + i * 35, 320, 35, COLOR_COOPER);
            gfx->setTextColor(COLOR_BG);
        } else {
            gfx->setTextColor(COLOR_TEXT);
        }
        gfx->drawString(categories[i].c_str(), 10, 45 + i * 35);
    }
}

void DisplayUI::drawPresetList(const String& category) {
    gfx->fillScreen(COLOR_BG);
    
    auto presetsInCat = _presets->getPresetsByCategory(category);
    
    gfx->setTextColor(COLOR_COOPER);
    gfx->setTextSize(2);
    gfx->drawString(category.c_str(), 10, 10);
    
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(1);
    int visible = 7;
    for (int i = _scrollOffset; i < _scrollOffset + visible && i < presetsInCat.size(); i++) {
        if (i == _selectedPreset) {
            gfx->fillRect(0, 40 + (i - _scrollOffset) * 35, 320, 35, COLOR_COOPER);
            gfx->setTextColor(COLOR_BG);
        } else {
            gfx->setTextColor(COLOR_TEXT);
        }
        String name = presetsInCat[i]->name;
        if (name.length() > 45) name = name.substring(0, 42) + "...";
        gfx->drawString(name.c_str(), 10, 45 + (i - _scrollOffset) * 35);
    }
}

void DisplayUI::drawPresetDetail(Preset* preset) {
    if (!preset) return;
    
    gfx->fillScreen(COLOR_BG);
    
    gfx->setTextColor(COLOR_COOPER);
    gfx->setTextSize(2);
    gfx->drawString(preset->name.c_str(), 10, 10);
    
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(1);
    gfx->drawString("Programs:", 10, 60);
    
    for (size_t i = 0; i < preset->programs.size() && i < 6; i++) {
        gfx->printf("  %s: %d freqs\n", 
            preset->programs[i].id.c_str(), 
            (int)preset->programs[i].frequencies.size());
    }
}

void DisplayUI::drawPlayer() {
    gfx->fillScreen(COLOR_BG);
    
    gfx->setTextColor(COLOR_GREEN);
    gfx->setTextSize(2);
    
    if (_engine->isPlaying()) {
        gfx->drawString("PLAYING", 10, 10);
    } else {
        gfx->drawString("PAUSED", 10, 10);
    }
    
    gfx->setTextColor(COLOR_CYAN);
    gfx->setTextSize(3);
    char buf[32];
    sprintf(buf, "%.1f Hz", _engine->getCurrentFrequency());
    gfx->drawString(buf, 10, 80);
    
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(1);
    
    unsigned long elapsed = _engine->getElapsedTime() / 1000;
    unsigned long duration = _engine->getDuration() / 1000;
    sprintf(buf, "%02lu:%02lu / %02lu:%02lu",
        elapsed / 60, elapsed % 60,
        duration / 60, duration % 60);
    gfx->drawString(buf, 10, 150);
    
    int barWidth = 280;
    int filledWidth = (elapsed * barWidth) / (duration > 0 ? duration : 1);
    gfx->fillRect(20, 180, barWidth, 10, COLOR_DIM);
    gfx->fillRect(20, 180, filledWidth, 10, COLOR_GREEN);
}