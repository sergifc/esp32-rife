#include "FrequencyEngine.h"

FrequencyEngine::FrequencyEngine() 
    : _currentPreset(nullptr)
    , _currentProgramIndex(0)
    , _currentFreqIndex(0)
    , _currentFreq(0)
    , _currentOffset(0)
    , _isPlaying(false)
    , _elapsedTime(0)
    , _duration(0)
    , _lastFreqChange(0)
    , _freqInterval(30000) {
}

void FrequencyEngine::setPreset(Preset* preset) {
    _currentPreset = preset;
    _currentProgramIndex = 0;
    _currentFreqIndex = 0;
    _elapsedTime = 0;
    stop();
    
    if (preset && preset->programs.size() > 0) {
        Program& prog = preset->programs[0];
        if (prog.frequencies.size() > 0) {
            _currentFreq = prog.frequencies[0].hz;
            _currentOffset = prog.frequencies[0].offset;
            updateOutput();
        }
    }
}

void FrequencyEngine::setProgram(int programIndex) {
    if (!_currentPreset) return;
    if (programIndex >= 0 && programIndex < _currentPreset->programs.size()) {
        _currentProgramIndex = programIndex;
        _currentFreqIndex = 0;
        _elapsedTime = 0;
        
        Program& prog = _currentPreset->programs[programIndex];
        if (prog.frequencies.size() > 0) {
            _currentFreq = prog.frequencies[0].hz;
            _currentOffset = prog.frequencies[0].offset;
            updateOutput();
        }
    }
}

void FrequencyEngine::play() {
    if (_currentPreset && _currentPreset->programs.size() > 0) {
        _isPlaying = true;
        _lastFreqChange = millis();
    }
}

void FrequencyEngine::pause() {
    _isPlaying = false;
}

void FrequencyEngine::stop() {
    _isPlaying = false;
    _elapsedTime = 0;
    _currentFreq = 0;
    _currentOffset = 0;
}

void FrequencyEngine::update() {
    if (!_isPlaying || !_currentPreset) return;
    
    unsigned long now = millis();
    
    if (_duration > 0 && _elapsedTime >= _duration) {
        stop();
        return;
    }
    
    if (now - _lastFreqChange >= _freqInterval) {
        nextFrequency();
        _lastFreqChange = now;
    }
    
    _elapsedTime += 10;
}

void FrequencyEngine::nextFrequency() {
    if (!_currentPreset) return;
    if (_currentPreset->programs.empty()) return;
    
    Program& prog = _currentPreset->programs[_currentProgramIndex];
    if (prog.frequencies.empty()) return;
    
    _currentFreqIndex = (_currentFreqIndex + 1) % prog.frequencies.size();
    _currentFreq = prog.frequencies[_currentFreqIndex].hz;
    _currentOffset = prog.frequencies[_currentFreqIndex].offset;
    
    updateOutput();
}

void FrequencyEngine::prevFrequency() {
    if (!_currentPreset) return;
    if (_currentPreset->programs.empty()) return;
    
    Program& prog = _currentPreset->programs[_currentProgramIndex];
    if (prog.frequencies.empty()) return;
    
    _currentFreqIndex = (_currentFreqIndex - 1 + prog.frequencies.size()) % prog.frequencies.size();
    _currentFreq = prog.frequencies[_currentFreqIndex].hz;
    _currentOffset = prog.frequencies[_currentFreqIndex].offset;
    
    updateOutput();
}

void FrequencyEngine::updateOutput() {
    Serial.print("Frequency: ");
    Serial.print(_currentFreq);
    Serial.print(" Hz, Offset: ");
    Serial.println(_currentOffset);
}
