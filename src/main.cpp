#include "speechRecognitionWorker.h"

int main(int argc, char **argv) {
    speechRecognitionWorker sr;

    // continuous mode, use keywordDetect
    sr.start("continuous", true);

    std::cout << "Ready.." << std::endl;

    sr.join();

    return 0;
}
