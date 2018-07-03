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
