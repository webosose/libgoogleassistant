#include <unistd.h>
#include <string.h>
#include <json-c/json.h>

#include "speechRecognitionWorker.h"

#include "logging.h"

speechRecognitionWorker::speechRecognitionWorker(eventHandler *parent):
eventHandler(parent),
mState(IDLE),
pWorkThread(NULL),
bIsStarted(false),
bIsContinuous(true),
bUseKeywordDetector(true),
pAudioCapture(NULL),
pAudioPlayback(NULL),
pKeywordDetector(NULL),
pGoogleAssistant(NULL),
pfnThrowEvent(NULL) {
}

speechRecognitionWorker::~speechRecognitionWorker() {
    stop();
    finalize();
}

void speechRecognitionWorker::initialize() {
    pAudioCapture  = new audioCapture;
    pAudioPlayback = new audioPlayback;

    pKeywordDetector = new keywordDetector(pAudioCapture, this);
    pGoogleAssistant = new googleAssistant(pAudioCapture, pAudioPlayback, this);
}

void speechRecognitionWorker::finalize() {
    if ( pKeywordDetector )     { delete pKeywordDetector; pKeywordDetector = NULL; }
    if ( pGoogleAssistant )     { delete pGoogleAssistant; pGoogleAssistant = NULL; }
    if ( pAudioCapture )        { delete pAudioCapture;    pAudioCapture = NULL; }
    if ( pAudioPlayback )       { delete pAudioPlayback;   pAudioPlayback = NULL; }
}

ERROR_CODE speechRecognitionWorker::start(char *mode, bool uKd) {
    if (!bIsStarted) {

        if (!strcmp(mode, "continuous"))  bIsContinuous = true;
        else if (!strcmp(mode, "single")) bIsContinuous = false;
        else return INVALID_PARAM;

        if (uKd) bUseKeywordDetector = true;
        else bUseKeywordDetector     = false;

        bIsStarted = true;

        initialize();
        if (!pWorkThread) pWorkThread = new pthread_t;
        if (pthread_create(pWorkThread, NULL, worker, this)) {
            finalize();
            bIsStarted = false;
            delete pWorkThread;
            pWorkThread = NULL;

            return UNKNOWN_ERROR;
        }

        return NONE;
    }

    return ALREADY_STARTED;
}

ERROR_CODE speechRecognitionWorker::stop() {
    if (bIsStarted) {
        bIsStarted = false;

        if (pKeywordDetector) pKeywordDetector->stop();
        if (pGoogleAssistant) pGoogleAssistant->stop();

        join();
        finalize();
        setState(IDLE);

        return NONE;
    }

    return NOT_STARTED;
}

bool speechRecognitionWorker::join() {
    if (pWorkThread) {
        int status;
        pthread_join(*pWorkThread, (void **)&status);
        delete pWorkThread;
        pWorkThread = NULL;
    } else return false;

    return true;
}

void* speechRecognitionWorker::worker(void *ctx) {
    // ctx is a pointer of parent thread.
    speechRecognitionWorker *p = (speechRecognitionWorker *)ctx;

    do {
        if (p->bUseKeywordDetector) {
            p->setState(WAITACTIVATION);
            if (!p->pKeywordDetector->start() || !p->bIsStarted) break;
        }

        if (!p->pGoogleAssistant->start()) break;
    } while (p->bIsContinuous);

    // This case means that error has occured.
    if (p->bIsStarted) {
        p->bIsStarted = false;
        p->finalize();
        p->setState(IDLE);
    }

    return NULL;
}

void speechRecognitionWorker::setState(STATE s) {
    mState = s;

    std::string value = std::string("{\"state\":\"") + stateStr(mState) + "\"}";
    throwEvent((void *)subscription_key_state, (void *)value.c_str());
}

void speechRecognitionWorker::catchEvent(void *key, void *value) {
    if (!strcmp((char *)key, subscription_key_state)) {
        json_object *object = json_tokener_parse((char *)value);
        if (is_error(object)) return;

        const char *state  = json_object_get_string(json_object_object_get(object, "state"));
        if (!strcmp(state, "recording")) setState(RECORDING);
        else if (!strcmp(state, "thinking")) setState(THINKING);
        else if (!strcmp(state, "answering")) setState(ANSWERING);

        return;
    }

    throwEvent(key, value);
}
