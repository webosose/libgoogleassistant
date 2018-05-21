#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "keywordDetector.h"

#define resource_filename "common.res"
#define model_snowboy "snowboy.umdl"
#define sensitivity_str "0.5"
#define audio_gain 1.0
#define apply_frontend false

bool keywordDetector::bIsKdFinished = false;

keywordDetector::keywordDetector(audioCapture *ac):
pAc(ac),
mDetector(resource_filename, model_snowboy) {
  mDetector.SetSensitivity(sensitivity_str);
  mDetector.SetAudioGain(audio_gain);
  mDetector.ApplyFrontend(apply_frontend);
}

keywordDetector::~keywordDetector() {
}

bool keywordDetector::startKeywordDetection() {
    pAc->flush();

    bIsKdFinished = false;

    while(!bIsKdFinished) {
        char *pcmBuffer = pAc->get();

        if(pcmBuffer) {
            int result = mDetector.RunDetection((int16_t *)pcmBuffer, pAc->getCapacity()/sizeof(int16_t));
            if (result == 1) {
                fprintf(stderr, ">>>>> Hotword detected! <<<<<\n");
                bIsKdFinished = true;
            }
        }
        else {
            fprintf(stderr, "audio capture error\n");
            break;
        }

        usleep(25000);
    }

    if (!bIsKdFinished) {
        return false;
    }

    return true;
}

void keywordDetector::stopKeywordDetection() {
    bIsKdFinished = true;
}
