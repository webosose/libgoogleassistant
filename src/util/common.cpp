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

#include "common.h"

namespace speechRecognition {
    char* stateStr(STATE s) {
        switch (s) {
            case IDLE:           return "idle";
            case WAITACTIVATION: return "waitActivation";
            case RECORDING:      return "recording";
            case THINKING:       return "thinking";
            case ANSWERING:      return "answering";
            default:             break;
        }

        return "Not allowed state";
    }

    char* errorStr(ERROR_CODE e) {
        switch (e) {
            case NONE:              return "Not error";
            case INVALID_PARAM:     return "Invalid parameters";
            case CONFIG_ERROR:      return "Invalid configurations";
            case AUTH_ERROR:        return "Authentication failed";
            case NETWORK_ERROR:     return "Network error has occurred";
            case NOT_SUPPORTED:     return "Not supported";
            case NOT_STARTED:       return "Service had not started";
            case ALREADY_STARTED:   return "Service had already started";
            case MICROPHONE_ERROR:  return "Microphone error has occurred";
            case AUDIO_ERROR:       return "Audio error has occurred";
            case CAMERA_ERROR:      return "Camera error has occurred";
            default:                break;
        }

        return "Unknown error";
    }

    char* subscription_key_state    = "SPEECHRECOGNITION_STATE";
    char* subscription_key_response = "SPEECHRECOGNITION_RESPONSE";
}
