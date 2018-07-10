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

#ifndef __GOOGLEASSISTANT_H__
#define __GOOGLEASSISTANT_H__

#include <string>
#include <pthread.h>
#include <grpc++/grpc++.h>

#include <google/assistant/embedded/v1alpha2/embedded_assistant.pb.h>
#include <google/assistant/embedded/v1alpha2/embedded_assistant.grpc.pb.h>

#include "common.h"
#include "audioCapture.h"
#include "audioPlayback.h"

using namespace speechRecognition;

using google::assistant::embedded::v1alpha2::EmbeddedAssistant;
using google::assistant::embedded::v1alpha2::AssistRequest;
using google::assistant::embedded::v1alpha2::AssistResponse;

class googleAssistant : public eventHandler {
public:
    googleAssistant(audioCapture *ac, audioPlayback *ap, eventHandler *parent = NULL);
    ~googleAssistant();

    bool start();
    void stop();

private:
    std::shared_ptr<grpc::Channel> createChannel(const std::string &host);

    static void* listenWorker(void *ctx);

    void postState(std::string state);
    void postError(ERROR_CODE e);
    void postResponse(std::string response);

private:
    audioCapture    *pAc;
    audioPlayback   *pAp;
    pthread_t       mListenThread;

    std::shared_ptr<grpc::ClientReaderWriter<AssistRequest, AssistResponse> > pStream;

    bool     bIsCaptureFinished;
    bool     bIsSrFinished;
    bool     bIsConversation;
};

#endif
