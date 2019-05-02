//
// Created by prostoichelovek on 01.05.19.
//

#include "Recognition.h"

Recognition::Recognition(std::string langModel, std::string dict, std::string jsgf, bool hideLog) {
    cmd_ln_t *config;
    config = cmd_ln_init(NULL, ps_args(), TRUE,
                                   "-hmm", langModel.c_str(), "-dict", dict.c_str(), "-jsgf", jsgf.c_str(),
                                   "-logfn", hideLog ? "/dev/null" : "/dev/stdout", NULL);
    deocder = ps_init(config);

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

    while (1) {
        // capture the number of frames in the audio buffer
        framesInBuf = ad_read(audioDev, audioBuf, 4096);
        // send the audio buffer to the pocketsphinx decoder
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
            if (hyp == NULL)
                return "";
            return hyp;
        }
    }
}

void Recognition::loop() {
    while(runLoop) {
        std::string decoded_speech = recognize_from_microphone();
        if (decoded_speech == "") continue;

        if(onRecognize) onRecognize(decoded_speech);
    }
}

void Recognition::start() {
    runLoop = true;
    loopTh = std::thread(&Recognition::loop, this);
}

void Recognition::wait() {
    if(runLoop && loopTh.joinable())
        loopTh.join();
}