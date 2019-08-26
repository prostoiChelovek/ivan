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

    inline int countOccurs(const std::string &str, const std::string &target) {
        int occurrences = 0;
        std::string::size_type pos = 0;
        while ((pos = str.find(target, pos)) != std::string::npos) {
            ++occurrences;
            pos += target.length();
        }
        return occurrences;
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

        // ignore speech that contains <s>
        // set to -1 for not use
        int maxNoizeOccurs = 0;

        bool pause = false;

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
