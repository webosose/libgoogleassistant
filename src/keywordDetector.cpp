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

/*
 * For fixing follow errors.
 *
 * error: undefined reference to 'snowboy::SnowboyDetect::SnowboyDetect(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
 * error: undefined reference to 'snowboy::SnowboyDetect::SetSensitivity(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
 *
 * The snowboy-detect library doesn't support cxx11
 *
 */
#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>

#include "keywordDetector.h"

#include "logging.h"

#define resource_filename "/etc/googleAssistant/common.res"
#define audio_gain 1.0
#define apply_frontend false

bool keywordDetector::bIsKdFinished = false;

keywordDetector::keywordDetector(audioCapture *ac, eventHandler *parent):
eventHandler(parent),
pAc(ac),
mDetector(resource_filename, getenv("KEYWORD_DETECT_MODEL")) {
  mDetector.SetSensitivity(getenv("KEYWORD_DETECT_SENSITIVITY"));
  mDetector.SetAudioGain(audio_gain);
  mDetector.ApplyFrontend(apply_frontend);
}

keywordDetector::~keywordDetector() {
}

bool keywordDetector::start() {
    pAc->flush();

    bIsKdFinished = false;

    while(!bIsKdFinished) {
        uint8_t *pcmBuffer = pAc->get();

        if(pcmBuffer) {
            int result = mDetector.RunDetection((int16_t *)pcmBuffer, pAc->getCapacity()/sizeof(int16_t));
            if (result >= 1) {
                GOOGLEAI_LOG_DEBUG("keyword detected");
                postKeywordIndex(result);
                bIsKdFinished = true;
            }
        }
        else {
            GOOGLEAI_LOG_ERROR("%s microphone error has occured.", __FUNCTION__);
            postError(MICROPHONE_ERROR);
            break;
        }

        usleep(100);
    }

    if (!bIsKdFinished) {
        return false;
    }

    return true;
}

void keywordDetector::stop() {
    bIsKdFinished = true;
}

void keywordDetector::postError(ERROR_CODE e) {
    // post error event
    char* value = g_strdup_printf("{\"errorCode\":%d,\"errorText\":\"%s\"}", e, errorStr(e));
    throwEvent((void *)subscription_key_response, (void *)value);
    g_free(value);
}

void keywordDetector::postKeywordIndex(int idx) {
    // post error event
    char* value = g_strdup_printf("{\"provider\":\"googleassistant\",\"response\":{\"keywordDetected\":%d}}", idx);
    throwEvent((void *)subscription_key_response, (void *)value);
    g_free(value);
}
