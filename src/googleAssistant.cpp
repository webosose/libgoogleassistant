// Copyright (c) 2018-2019 LG Electronics, Inc.
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "googleAssistant.h"

#include "logging.h"

using google::assistant::embedded::v1alpha2::AudioInConfig;
using google::assistant::embedded::v1alpha2::AudioOutConfig;
using google::assistant::embedded::v1alpha2::AssistResponse_EventType_END_OF_UTTERANCE;
using google::assistant::embedded::v1alpha2::DialogStateOut_MicrophoneMode;
using google::assistant::embedded::v1alpha2::DialogStateOut_MicrophoneMode_CLOSE_MICROPHONE;
using google::assistant::embedded::v1alpha2::DialogStateOut_MicrophoneMode_DIALOG_FOLLOW_ON;

googleAssistant::googleAssistant(audioCapture *ac, audioPlayback *ap, eventHandler *parent):
eventHandler(parent),
pAc(ac),
pAp(ap) {
}

googleAssistant::~googleAssistant() {
}

// Creates a channel to be connected to Google.
std::shared_ptr<grpc::Channel> googleAssistant::createChannel(const std::string& host) {
    // Default is /usr/share/grpc/root.pem
    grpc::SslCredentialsOptions ssl_opts = {"", "", ""};

    auto creds = grpc::SslCredentials(ssl_opts);
    std::string server = host + ":443";
    GOOGLEAI_LOG_INFO("%s create custom channel(%s)", __FUNCTION__, server.c_str());

    grpc::ChannelArguments channel_args;

    return CreateCustomChannel(server, creds, channel_args);
}

bool googleAssistant::start() {
    bIsConversation = true;

    do {
        postState("recording");

        bIsCaptureFinished = false;

        // 1. Read credentials file and check it.
        char *credentials = getenv("GOOGLEAI_CREDENTIALS");
        std::ifstream crefile(credentials);
        if (!crefile) {
            GOOGLEAI_LOG_ERROR("%s credentials file \"%s\" does not exist.", __FUNCTION__, credentials);
            postError(CONFIG_ERROR);
            return false;
        }

        std::stringstream crebuff;
        crebuff << crefile.rdbuf();
        std::string cre = crebuff.str();

        std::shared_ptr<grpc::CallCredentials> call_credentials(grpc::GoogleRefreshTokenCredentials(cre));
        if (call_credentials.get() == nullptr) {
            GOOGLEAI_LOG_ERROR("%s credentials file \"%s\" is invalid.", __FUNCTION__, credentials);
            postError(AUTH_ERROR);
            return false;
        }

        // 2. Connect to google assistant api server
        char *endpoint = getenv("GOOGLEAI_ENDPOINT");
        if (!endpoint) {
            GOOGLEAI_LOG_ERROR("%s endpoint \"%s\" is invalid.", __FUNCTION__, endpoint);
            postError(CONFIG_ERROR);
            return false;
        }
        std::shared_ptr<grpc::Channel> channel = createChannel(endpoint);
        std::unique_ptr<EmbeddedAssistant::Stub> assistant(EmbeddedAssistant::NewStub(channel));

        // 3. Begin a stream.
        grpc::ClientContext context;
        context.set_fail_fast(false);
        context.set_credentials(call_credentials);
        pStream = std::move(assistant->Assist(&context));

        // 4. Write config in first stream.
        AssistRequest request;
        auto* assist_config = request.mutable_config();
        assist_config->mutable_audio_in_config()->set_encoding(AudioInConfig::LINEAR16);
        assist_config->mutable_audio_in_config()->set_sample_rate_hertz(pAc->getBitrate());
        assist_config->mutable_audio_out_config()->set_encoding(AudioOutConfig::LINEAR16);
        assist_config->mutable_audio_out_config()->set_sample_rate_hertz(pAp->getBitrate());

        char *language = getenv("GOOGLEAI_LANGUAGE");
        if (!language) {
            GOOGLEAI_LOG_ERROR("%s need language code", __FUNCTION__);
            postError(CONFIG_ERROR);
            return false;
        }
        assist_config->mutable_dialog_state_in()->set_language_code(language);

        char *device_model = getenv("GOOGLEAI_DEVICE_MODEL");
        char *device_id    = getenv("GOOGLEAI_DEVICE_ID");
        if (!device_model || !device_id) {
            GOOGLEAI_LOG_ERROR("%s need device model id and device id", __FUNCTION__);
            postError(CONFIG_ERROR);
            return false;
        }
        assist_config->mutable_device_config()->set_device_model_id(device_model);
        assist_config->mutable_device_config()->set_device_id(device_id);

        pStream->Write(request);
        GOOGLEAI_LOG_INFO("%s wrote first request: %s", __FUNCTION__, request.ShortDebugString().c_str());

        if ( pthread_create(&mListenThread, NULL, listenWorker, this) ) break;

        while(!bIsCaptureFinished) {
            uint8_t *pcmBuffer = pAc->get();

            if(pcmBuffer) {
                request.set_audio_in(pcmBuffer, pAc->getCapacity());
                pStream->Write(request);
            } else {
                GOOGLEAI_LOG_ERROR("%s microphone error has occured.", __FUNCTION__);
                bIsSrFinished = true;
                postError(MICROPHONE_ERROR);
                break;
            }

            usleep(100);
        }

        pStream->WritesDone();

        while (!bIsSrFinished) {
            // clear audio input buffer during TTS
            pAc->get();
            usleep(100);
        }

        int status;
        pthread_join(mListenThread, (void **)&status);

        if (!bIsCaptureFinished) break;
    } while (bIsConversation);

    if (!bIsCaptureFinished) return false;

    return true;
}

void googleAssistant::stop() {
    bIsCaptureFinished  = true;
    bIsSrFinished       = true;
    bIsConversation     = false;
}

void googleAssistant::postState(std::string state) {
    // post state change event
    std::string value = std::string("{\"state\":\"") + state + "\"}";
    throwEvent((void *)subscription_key_state, (void *)value.c_str());
}

void googleAssistant::postError(ERROR_CODE e) {
    // post error event
    std::string value = std::string("{\"errorCode\":") + std::to_string(e) +
                                    ",\"errorText\":\"" + errorStr(e) + "\"}";
    throwEvent((void *)subscription_key_response, (void *)value.c_str());
}

void googleAssistant::postResponse(std::string response) {
    // post error event
    std::string value = std::string("{\"provider\":\"googleassistant\",\"response\":") + response + "}";
    throwEvent((void *)subscription_key_response, (void *)value.c_str());
}

void* googleAssistant::listenWorker(void *ctx) {
    GOOGLEAI_LOG_INFO("%s waiting for response...", __FUNCTION__);

    googleAssistant* g = (googleAssistant *)ctx;

    g->bIsSrFinished = false;

    DialogStateOut_MicrophoneMode micMode = DialogStateOut_MicrophoneMode_CLOSE_MICROPHONE;

    AssistResponse response;
    while (!g->bIsSrFinished && g->pStream->Read(&response)) {  // Returns false when no more to read.
        if (response.event_type() == AssistResponse_EventType_END_OF_UTTERANCE && !g->bIsCaptureFinished) {
            g->bIsCaptureFinished = true;
            g->postState("thinking");
        }

        if (response.has_audio_out()) {
            g->bIsCaptureFinished = true;

            uint8_t *buff     = response.audio_out().audio_data().c_str();
            size_t  audioSize = response.audio_out().audio_data().length();
            if (!g->pAp->put(buff, audioSize)) {
                GOOGLEAI_LOG_ERROR("audio playback error has occured.");
                g->postError(AUDIO_ERROR);
            }
        } 

        if (response.has_dialog_state_out()) {
            GOOGLEAI_LOG_DEBUG("Response text: %s", response.dialog_state_out().supplemental_display_text().c_str());
            micMode = response.dialog_state_out().microphone_mode();
            g->postState("answering");
            g->postResponse(std::string("{\"displayText\":\"") + response.dialog_state_out().supplemental_display_text() + "\"}");
        }

        if (response.has_device_action()) {
            GOOGLEAI_LOG_DEBUG("Device action: %s", response.device_action().device_request_json().c_str());
            g->postResponse(std::string("{\"deviceAction\":") + response.device_action().device_request_json() + "}");
        }

        std::string partial;
        for (int i = 0; i < response.speech_results_size(); i++) {
            if (response.speech_results(i).transcript().length()) {
                partial += response.speech_results(i).transcript();
            }
        }

        if (partial.length()) {
            GOOGLEAI_LOG_DEBUG("Partial: %s", partial.c_str());
            g->postResponse(std::string("{\"partial\":\"") + partial + "\"}");
        }
    }

    grpc::Status status = g->pStream->Finish();
    if (!status.ok()) {
        // Report the RPC failure.
        GOOGLEAI_LOG_WARNING("%s finish failed", __FUNCTION__);
    }

    if (micMode == DialogStateOut_MicrophoneMode_DIALOG_FOLLOW_ON) {
        g->bIsConversation = true;
    } else {
        g->bIsConversation = false;
    }

    g->bIsSrFinished = true;

    return NULL;
}
