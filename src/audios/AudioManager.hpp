#pragma once

#include "../audio/miniaudio.h"
#include <string>

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    bool init();

    void play(const std::string& file);

private:
    ma_engine engine;
    bool initialized = false;
};