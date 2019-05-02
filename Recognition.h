//
// Created by prostoichelovek on 01.05.19.
//

#ifndef SPEECH_RECOGNITION_H
#define SPEECH_RECOGNITION_H

#include <string>
#include <functional>
#include <thread>
#include <condition_variable>
#include <future>

#include <pocketsphinx.h>
#include <sphinxbase/ad.h>

using CallbackFn = std::function<void(std::string)>;

// http://www.robotrebels.org/index.php?topic=239.0
class Recognition {
public:
    uint8 speech_started, in_speech;
    CallbackFn onRecognize;
    bool runLoop = false;

    Recognition(std::string langModel, std::string dict, std::string jsgf, bool hideLog = true);
    ~Recognition();

    std::string recognize_from_microphone();

    void start();
    void wait();

private:
    ps_decoder_t *deocder;
    ad_rec_t *audioDev;
    int16 audioBuf[4096];
    int32 framesInBuf;

    std::thread loopTh;

    void loop();
};


#endif //SPEECH_RECOGNITION_H
