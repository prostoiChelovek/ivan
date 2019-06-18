#include <iostream>

#include "speaking.h"
#include "Recognition.h"


int main() {
    RHSpeaker speaker("anna");

    SpeechRecognition::Recognition rec("../data/zero_ru_cont_8k_v3/zero_ru.cd_cont_4000", "../test.dic",
                                       "../data/gram.jsgf", "../data/kws.kwlist", false);
    SpeechRecognition::CallbackFn onKw = [&speaker, &rec](std::string str) {
        speaker.say("Слушаю...");
    };
    SpeechRecognition::CallbackFn onRec = [&speaker, &rec](std::string str) {
        std::cout << "You said: " << str << std::endl;
    };
    rec.onKw = onKw;
    rec.onRecognize = onRec;

    rec.start();
    speaker.say("Говорите");
    std::cout << "You can speak" << std::endl;

    rec.wait();
    return EXIT_SUCCESS;
}

