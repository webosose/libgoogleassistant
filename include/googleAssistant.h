#ifndef __GOOGLEASSISTANT_H__
#define __GOOGLEASSISTANT_H__

#include <string>
#include <pthread.h>
#include <grpc++/grpc++.h>

#include <google/assistant/embedded/v1alpha2/embedded_assistant.pb.h>
#include <google/assistant/embedded/v1alpha2/embedded_assistant.grpc.pb.h>

#include "audioCapture.h"
#include "audioPlayback.h"

using google::assistant::embedded::v1alpha2::EmbeddedAssistant;
using google::assistant::embedded::v1alpha2::AssistRequest;
using google::assistant::embedded::v1alpha2::AssistResponse;

class googleAssistant {
public:
    googleAssistant(audioCapture *ac, audioPlayback *ap);
    ~googleAssistant();

    bool start();
    void stop();

private:
    std::shared_ptr<grpc::Channel> createChannel(const std::string &host);

    static void* listenWorker(void *ctx);

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
