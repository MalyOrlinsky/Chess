#define MINIAUDIO_IMPLEMENTATION
#include "AudioManager.hpp"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
    if (initialized)
        ma_engine_uninit(&engine);
}

bool AudioManager::init()
{
    if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
        return false;

    initialized = true;
    return true;
}

void AudioManager::play(const std::string& file)
{
    if (!initialized)
        return;

    ma_engine_play_sound(&engine, file.c_str(), nullptr);
}