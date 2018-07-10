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
