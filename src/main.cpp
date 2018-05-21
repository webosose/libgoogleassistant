#include "keywordDetector.h"

int main(int argc, char **argv) {
    audioCapture mAc;
    keywordDetector kd(&mAc);

    fprintf(stderr, "Ready..\n");

    do {
        if ( !kd.startKeywordDetection() ) break;
    } while(true);

    return 0;
}
