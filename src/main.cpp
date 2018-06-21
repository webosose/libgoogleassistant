#include "keywordDetector.h"
#include "googleAssistant.h"

int main(int argc, char **argv) {
    audioCapture mAc;
    audioPlayback mAp;

    keywordDetector kd(&mAc);
    googleAssistant ga(&mAc, &mAp);

    do {
        fprintf(stderr, "Ready..\n");
        if ( !kd.start() ) break;
        if ( !ga.start() ) break;
    } while(true);

    return 0;
}
