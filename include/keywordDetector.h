#ifndef __KEYWORDDETECTOR_H__
#define __KEYWORDDETECTOR_H__

#include "common.h"
#include "audioCapture.h"
#include "snowboy-detect.h"

using namespace speechRecognition;

class keywordDetector : public eventHandler {
public:
    keywordDetector(audioCapture *ac, eventHandler *parent = NULL);
    ~keywordDetector();

    bool start();
    void stop();

private:
    void postError(ERROR_CODE e);

private:
    audioCapture            *pAc;
    snowboy::SnowboyDetect  mDetector;

    static bool             bIsKdFinished;
};

#endif
