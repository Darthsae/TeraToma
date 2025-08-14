#include <TeraToma/Assets/AudioAsset.h>
#include <iostream>

using namespace TeraToma::Assets;

AudioAsset::AudioAsset(std::string a_id, std::string a_name) {
    this->id = a_id;
    this->name = a_name;
}

void AudioAsset::Load(SDL_AudioDeviceID a_audioDevice, std::string a_path) {
    if (SDL_LoadWAV(a_path.c_str(), &wavSpec, &wavStart, &wavLength) == NULL) {
        std::cout << "Someone ###### up with audio at " << a_path.c_str() << ".\n - Error is here: " << SDL_GetError() << std::endl;
    }

    stream = SDL_CreateAudioStream(&wavSpec, NULL);

    if (!SDL_BindAudioStream(a_audioDevice, stream)) {
        std::cout << "Someone ###### up with audio at " << a_path.c_str() << ".\n - Error is here: " << SDL_GetError() << std::endl;
    }
}

void AudioAsset::Play(SDL_AudioDeviceID a_audioDevice) {
    SDL_PutAudioStreamData(stream, wavStart, (int)wavLength);
}

void AudioAsset::Unload() {
    SDL_DestroyAudioStream(stream);
    SDL_free(wavStart);
}   