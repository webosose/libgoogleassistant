// Copyright (c) 2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef __SPEECHRECOGNITIONWORKER_H__
#define __SPEECHRECOGNITIONWORKER_H__

#include <pthread.h>

#include "common.h"
#include "keywordDetector.h"
#include "googleAssistant.h"

using namespace speechRecognition;

typedef bool (*pfnEventCB)(void* subscriptionKey, void* payload);

class speechRecognitionWorker : public eventHandler {
public:
    speechRecognitionWorker(eventHandler *parent = NULL);
    ~speechRecognitionWorker();

    ERROR_CODE start(char *mode, bool uKd);
    ERROR_CODE stop();

    bool       join();

    STATE getState() { return mState; }

public:
    void register_event_Callback(pfnEventCB pCB) { pfnThrowEvent = pCB; }

private:
    static void* worker(void *ctx);

    void initialize();
    void finalize();

    void setState(STATE s);

    virtual void throwEvent(void *key, void *value) override { if (pfnThrowEvent) pfnThrowEvent(key, value); }
    virtual void catchEvent(void *key, void *value) override;

private:
    STATE               mState;

    pthread_t          *pWorkThread;

    bool                bIsStarted;
    bool                bIsContinuous;
    bool                bUseKeywordDetector;

    audioCapture       *pAudioCapture;
    audioPlayback      *pAudioPlayback;

    keywordDetector    *pKeywordDetector;
    googleAssistant    *pGoogleAssistant;

    pfnEventCB          pfnThrowEvent;
};

#endif
