#include <iostream>

#include "speaking.h"
#include "Recognition.h"


int main() {
    RHSpeaker speaker("irina");

    SpeechRecognition::Recognition rec("../data/cmusphinx-en-us-5.2", "../dict.en.dic",
                                       "../gram-en.jsgf", "../kws-en.kwlist", false);
    SpeechRecognition::CallbackFn onKw = [&speaker, &rec](std::string str) {
        speaker.say("Слушаю...");
        std::cout << "Phase: " << str << std::endl;
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

