#include <stdio.h>
#include <string.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#include "audioCapture.h"

audioCapture::audioCapture(uint32_t capacity, uint32_t rate, uint32_t channels) :
pHandle(NULL),
pBuff(NULL),
mCapacity(capacity),
mBitrate(rate),
mChannels(channels) {
    initialize();
}

audioCapture::~audioCapture() {
    finalize();
}

void audioCapture::initialize() {
    pBuff = new uint8_t [mCapacity];

    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = (uint32_t)mBitrate,
        .channels= (uint8_t)mChannels
    };

    // for low latency. (response time)
    pa_buffer_attr attr;
    memset(&attr, 0x00, sizeof(pa_buffer_attr));
    attr.maxlength = attr.prebuf  = attr.minreq = (uint32_t) -1; // default value
    attr.fragsize  = attr.tlength = mCapacity;

    int error;

    if (!(pHandle = pa_simple_new(NULL, "PulseCapture", PA_STREAM_RECORD, NULL, "record", &ss, NULL, &attr, &error))) {
        printf("%s failed. %s\n",__FUNCTION__, pa_strerror(error));
    }
}

void audioCapture::finalize() {
    if (pHandle) {
        pa_simple_free(pHandle);
        pHandle = NULL;
    }

    if (pBuff) {
        delete pBuff;
        pBuff = NULL;
    }
}

void audioCapture::flush() {
    if (pHandle) {
        pa_simple_flush(pHandle, NULL);
    }
}

uint8_t* audioCapture::get(size_t size) {
    if (!size) size = mCapacity;

    if (pHandle && size <= mCapacity) {
        int error;

        if (pa_simple_read(pHandle, pBuff, size, &error) < 0) {
            printf("%s failed. %s\n",__FUNCTION__, pa_strerror(error));
            return NULL;
        }

        return pBuff;
    }

    return NULL;
}
