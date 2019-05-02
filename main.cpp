#include <iostream>

#include "speaking.h"
#include "Recognition.h"

int main() {
    RHSpeaker speaker("alan");

    Recognition rec("../data/zero_ru.cd_cont_4000", "../data/test.dic", "../data/test.jsgf");
    CallbackFn cb = [&speaker, &rec](std::string str){
        std::cout << "You said: " << str << std::endl;
    };
    rec.onRecognize = cb;

    rec.start();
    speaker.say("You can speak");
    std::cout << "You can speak" << std::endl;

    rec.wait();
    return EXIT_SUCCESS;
}

