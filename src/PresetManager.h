#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <string>

struct Frequency {
    float hz;
    uint8_t offset;
};

struct Program {
    String id;
    String name;
    std::vector<Frequency> frequencies;
};

struct Preset {
    String name;
    String mode;
    String category;
    String animal_category;
    String description;
    std::vector<Program> programs;
};

class PresetManager {
public:
    PresetManager();
    void loadFromSPIFFS();
    void loadFromJson(const String& jsonStr);
    
    std::vector<String> getCategories();
    std::vector<Preset*> getPresetsByCategory(const String& category);
    Preset* getPreset(int index);
    int getPresetCount();
    
private:
    std::vector<Preset> _presets;
    std::vector<String> _categories;
    
    void parsePreset(const JsonObjectConst& obj);
};
