//
// Created by prostoichelovek on 01.05.19.
//

#include <iostream>
#include "Recognition.h"

namespace SpeechRecognition {

    Recognition::Recognition(std::string langModel, std::string dict, std::string jsgf,
                             std::string kws, bool hideLog) {
        cmd_ln_t *config;
        config = cmd_ln_init(nullptr, ps_args(), TRUE,
                             "-hmm", langModel.c_str(), "-dict", dict.c_str(),
                             "-logfn", hideLog ? "/dev/null" : "/dev/stdout", NULL);
        deocder = ps_init(config);

        if (!jsgf.empty()) {
            ps_set_jsgf_file(deocder, searchTypeStr(JSGF).c_str(), jsgf.c_str());
            switchMode(JSGF);
        }
        if (!kws.empty()) {
            ps_set_kws(deocder, searchTypeStr(KWS).c_str(), kws.c_str());
            switchMode(KWS);
        }

        audioDev = ad_open_sps((int) cmd_ln_float32_r(config, "-samprate"));
    }

    Recognition::~Recognition() {
        ad_close(audioDev);
        runLoop = false;
    }

    std::string Recognition::recognize_from_microphone() {
        ad_start_rec(audioDev);                             // start recording
        ps_start_utt(deocder);                              // mark the start of the utterance
        speech_started = FALSE;                             // clear the speech_started flag

        while (true) {
            // capture the number of frames in the audio buffer
            framesInBuf = ad_read(audioDev, audioBuf, 4096);
            if (framesInBuf == -1)
                break;

            ps_process_raw(deocder, audioBuf, framesInBuf, FALSE, FALSE);
            in_speech = ps_get_in_speech(deocder);

            if (in_speech && !speech_started) {
                speech_started = TRUE;
            }

            if (!in_speech && speech_started) {
                ps_end_utt(deocder);
                ad_stop_rec(audioDev);

                // query pocketsphinx for "hypothesis" of decoded statement
                char const *hyp = ps_get_hyp(deocder, nullptr);

                if (hyp == nullptr)
                    return "";
                return hyp;
            }
        }
        return "";
    }

    void Recognition::switchMode(const SearchType &st) {
        ps_set_search(deocder, searchTypeStr(st).c_str());
        currentSt = st;
    }

    void Recognition::loop() {
        while (runLoop) {
            std::string decoded_speech = recognize_from_microphone();
            switchMode(currentSt);
            if (decoded_speech.empty()) continue;
            if (maxNoizeOccurs != -1) {
                if (countOccurs(decoded_speech, "<s>") > maxNoizeOccurs) continue;
            }

            if (ps_get_search(deocder) == searchTypeStr(KWS)) {
                switchMode(JSGF);
                if (onKw)
                    onKw(decoded_speech);
                std::thread([&]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(int(kwTimeout * 1000)));
                    currentSt = KWS;
                }).detach();
                continue;
            }
            if (onRecognize) {
                if (ps_get_search(deocder) == searchTypeStr(JSGF)) {
                    onRecognize(decoded_speech);
                    switchMode(KWS);
                }
            }
        }
    }

    void Recognition::start() {
        runLoop = true;
        loopTh = std::thread(&Recognition::loop, this);
    }

    void Recognition::wait() {
        if (runLoop && loopTh.joinable())
            loopTh.join();
    }
}