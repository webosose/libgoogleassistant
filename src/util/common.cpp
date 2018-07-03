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
