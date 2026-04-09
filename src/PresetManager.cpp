#include "PresetManager.h"
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

PresetManager::PresetManager() {}

void PresetManager::loadFromSPIFFS() {
    File file = SPIFFS.open("/frequencies.json", "r");
    if (!file) {
        Serial.println("Failed to open frequencies.json");
        return;
    }

    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);
    file.close();

    loadFromJson(String(buf.get()));
}

void PresetManager::loadFromJson(const String& jsonStr) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonStr);
    
    if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return;
    }

    JsonArrayConst presetsArray = doc["presets"];
    if (!presetsArray.isNull()) {
        for (JsonObjectConst presetObj : presetsArray) {
            parsePreset(presetObj);
        }
    }

    JsonArrayConst categoriesArray = doc["categories"];
    if (!categoriesArray.isNull()) {
        for (const char* cat : categoriesArray) {
            _categories.push_back(String(cat));
        }
    }

    Serial.print("Loaded ");
    Serial.print(_presets.size());
    Serial.println(" presets");
}

void PresetManager::parsePreset(const JsonObjectConst& obj) {
    Preset preset;
    preset.name = obj["name"].as<String>();
    preset.mode = obj["mode"].as<String>();
    preset.category = obj["category"].as<String>();
    preset.animal_category = obj["animal_category"].as<String>();
    preset.description = obj["description"].as<String>();

    JsonArrayConst programsArray = obj["programs"];
    if (!programsArray.isNull()) {
        for (JsonObjectConst progObj : programsArray) {
            Program program;
            program.id = progObj["id"].as<String>();
            program.name = progObj["name"].as<String>();

            JsonArrayConst freqsArray = progObj["frequencies"];
            if (!freqsArray.isNull()) {
                for (JsonObjectConst freqObj : freqsArray) {
                    Frequency freq;
                    freq.hz = freqObj["hz"].as<float>();
                    freq.offset = freqObj["offset"].as<uint8_t>();
                    program.frequencies.push_back(freq);
                }
            }
            preset.programs.push_back(program);
        }
    }

    _presets.push_back(preset);
}

std::vector<String> PresetManager::getCategories() {
    return _categories;
}

std::vector<Preset*> PresetManager::getPresetsByCategory(const String& category) {
    std::vector<Preset*> result;
    for (auto& preset : _presets) {
        if (preset.animal_category == category) {
            result.push_back(&preset);
        }
    }
    return result;
}

Preset* PresetManager::getPreset(int index) {
    if (index >= 0 && index < _presets.size()) {
        return &_presets[index];
    }
    return nullptr;
}

int PresetManager::getPresetCount() {
    return _presets.size();
}
