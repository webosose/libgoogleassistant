#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstddef>

namespace speechRecognition {
    enum STATE {
        IDLE,
        WAITACTIVATION,
        RECORDING,
        THINKING,
        ANSWERING
    };

    char* stateStr(STATE s);

    enum ERROR_CODE {
        NONE                =  0,
        UNKNOWN_ERROR       = -1,
        INVALID_PARAM       = -2,
        CONFIG_ERROR        = -3,
        AUTH_ERROR          = -4,
        NETWORK_ERROR       = -5,
        NOT_SUPPORTED       = -6,
        NOT_STARTED         = -7,
        ALREADY_STARTED     = -8,
        MICROPHONE_ERROR    = -9,
        AUDIO_ERROR         = -10,
        CAMERA_ERROR        = -11
    };

    char* errorStr(ERROR_CODE e);

    extern char* subscription_key_state;
    extern char* subscription_key_response;
};

class eventHandler {
public:
    eventHandler(eventHandler *parent = NULL): pParent(parent) {}
    ~eventHandler() {}

    void setParent(eventHandler *parent) { pParent = parent; }

    virtual void throwEvent(void *key, void *value) { if (pParent) pParent->catchEvent(key, value); }
    virtual void catchEvent(void *key, void *value) { throwEvent(key, value); }

protected:
    eventHandler *pParent;
};

#endif
