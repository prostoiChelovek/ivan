//
// Created by prostoichelovek on 01.05.19.
//

#include "speaking.h"

audio_player::audio_player(const std::string &path) {
    if (!path.empty()) {
        stream.set_backend(RHVoice::audio::backend_file);
        stream.set_device(path);
    }
}

bool audio_player::set_sample_rate(int sample_rate) {
    try {
        if (stream.is_open() && (stream.get_sample_rate() != sample_rate))
            stream.close();
        stream.set_sample_rate(sample_rate);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool audio_player::play_speech(const short *samples, size_t count) {
    try {
        if (!stream.is_open())
            stream.open();
        stream.write(samples, count);
        return true;
    }
    catch (...) {
        stream.close();
        return false;
    }
}


void audio_player::finish() {
    if (stream.is_open())
        stream.drain();
}

RHSpeaker::RHSpeaker(std::string voice) {
    eng = RHVoice::smart_ptr<RHVoice::engine>(new RHVoice::engine);
    setVoice(std::move(voice));
}

void RHSpeaker::setVoice(std::string voice) {
    profile = eng->create_voice_profile(voice);
}

void RHSpeaker::say(std::string text) {
    std::unique_ptr<RHVoice::document> doc = RHVoice::document::create_from_plain_text(eng, text.begin(), text.end(), RHVoice::content_text,
                                                                     profile);
    doc->set_owner(player);
    doc->synthesize();
    player.finish();
}
