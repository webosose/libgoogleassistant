#ifndef __KEYWORDDETECTOR_H__
#define __KEYWORDDETECTOR_H__

#include "audioCapture.h"
#include "snowboy-detect.h"

class keywordDetector {
public:
    keywordDetector(audioCapture *ac);
    ~keywordDetector();

    bool startKeywordDetection();
    void stopKeywordDetection();

private:
    audioCapture            *pAc;
    snowboy::SnowboyDetect  mDetector;

    static bool             bIsKdFinished;
};

#endif
