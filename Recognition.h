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

namespace SpeechRecognition {
#include <pocketsphinx.h>
#include <sphinxbase/ad.h>

    using CallbackFn = std::function<void(std::string)>;

    enum SearchType {
        JSGF, KWS
    };

    inline std::string searchTypeStr(const SearchType &st) {
        if (st == JSGF)
            return "jsgf";
        if (st == KWS)
            return "kws";
        return "";
    }

// http://www.robotrebels.org/index.php?topic=239.0
    class Recognition {
    public:
        ps_decoder_t *deocder;

        uint8 speech_started, in_speech;
        CallbackFn onRecognize;
        CallbackFn onKw;
        float kwTimeout = 5.0;
        bool runLoop = false;

        Recognition(std::string langModel, std::string dict, std::string jsgf = "",
                    std::string kws = "", bool hideLog = true);

        ~Recognition();

        std::string recognize_from_microphone();

        // jsgf / kws
        void switchMode(const SearchType &st);

        void start();

        void wait();

    private:
        ad_rec_t *audioDev;
        int16 audioBuf[4096];
        int32 framesInBuf;
        SearchType currentSt = KWS;

        std::thread loopTh;

        void loop();
    };

}

#endif //SPEECH_RECOGNITION_H
