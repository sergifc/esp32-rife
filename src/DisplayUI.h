#pragma once

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "config.h"
#include "PresetManager.h"
#include "FrequencyEngine.h"

using namespace lgfx;
extern LGFX lcd;

class DisplayUI {
public:
    DisplayUI(PresetManager* presets, FrequencyEngine* engine);
    void update();
    void drawHome();
    void drawCategorySelect();
    void drawPresetList(const String& category);
    void drawPresetDetail(Preset* preset);
    void drawPlayer();
    
private:
    PresetManager* _presets;
    FrequencyEngine* _engine;
    int _currentScreen;
    int _selectedCategory;
    int _selectedPreset;
    int _scrollOffset;
    
    static const uint32_t COLOR_BG = 0x0a0a0f;
    static const uint32_t COLOR_COOPER = 0xb87333;
    static const uint32_t COLOR_SHIMMER = 0xc9a227;
    static const uint32_t COLOR_CYAN = 0x00fff7;
    static const uint32_t COLOR_GREEN = 0x00ff88;
    static const uint32_t COLOR_TEXT = 0xffffff;
    static const uint32_t COLOR_DIM = 0x666666;
};