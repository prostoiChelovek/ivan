//
// Created by prostoichelovek on 01.05.19.
//

#ifndef SPEECH_SPEAKING_H
#define SPEECH_SPEAKING_H


#include "RHVoice/core/smart_ptr.hpp"
#include "RHVoice/core/engine.hpp"
#include "RHVoice/core/document.hpp"
#include "RHVoice/audio.hpp"

// https://github.com/Olga-Yakovleva/RHVoice/blob/master/src/test/test.cpp
class audio_player : public RHVoice::client {
public:
    explicit audio_player(const std::string &path);

    audio_player() = default;

    bool play_speech(const short *samples, size_t count);

    void finish();

    bool set_sample_rate(int sample_rate);

private:
    RHVoice::audio::playback_stream stream;
};


class RHSpeaker {
public:
    explicit RHSpeaker(std::string voice);

    void setVoice(std::string voice);

    void say(std::string text);

private:
    audio_player player;
    RHVoice::smart_ptr<RHVoice::engine> eng;
    RHVoice::voice_profile profile;
};

#endif //SPEECH_SPEAKING_H
