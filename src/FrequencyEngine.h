#pragma once

#include <Arduino.h>
#include "PresetManager.h"

class FrequencyEngine {
public:
    FrequencyEngine();
    
    void setPreset(Preset* preset);
    void setProgram(int programIndex);
    void play();
    void pause();
    void stop();
    void update();
    
    bool isPlaying() const { return _isPlaying; }
    float getCurrentFrequency() const { return _currentFreq; }
    int getCurrentProgramIndex() const { return _currentProgramIndex; }
    int getCurrentFrequencyIndex() const { return _currentFreqIndex; }
    unsigned long getElapsedTime() const { return _elapsedTime; }
    unsigned long getDuration() const { return _duration; }
    void setDuration(unsigned long seconds) { _duration = seconds * 1000; }
    
    void nextFrequency();
    void prevFrequency();
    
private:
    Preset* _currentPreset;
    int _currentProgramIndex;
    int _currentFreqIndex;
    float _currentFreq;
    uint8_t _currentOffset;
    bool _isPlaying;
    unsigned long _elapsedTime;
    unsigned long _duration;
    unsigned long _lastFreqChange;
    unsigned long _freqInterval;
    
    void updateOutput();
};
