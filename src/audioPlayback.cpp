#include <stdio.h>
#include <string.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#include "audioPlayback.h"

audioPlayback::audioPlayback(uint32_t capacity, uint32_t rate, uint32_t channels):
pHandle(NULL),
mCapacity(capacity),
mBitrate(rate),
mChannels(channels) {
    initialize();
}

audioPlayback::~audioPlayback() {
    finalize();
}

void audioPlayback::initialize() {
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

    if (!(pHandle = pa_simple_new(NULL, "PulsePlayback", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, &attr, &error))) {
        fprintf(stderr, "%s failed. %s\n",__FUNCTION__, pa_strerror(error));
    }
}

void audioPlayback::finalize() {
    if (pHandle) {
        pa_simple_free(pHandle);
        pHandle = NULL;
    }
}

void audioPlayback::flush() {
    if (pHandle) {
        pa_simple_flush(pHandle, NULL);
    }
}

bool audioPlayback::put(uint8_t *buff, size_t size) {
    if (pHandle) {
        int error;

        size_t leftSize = size;
        while ((int32_t)leftSize > 0) {
            if (pa_simple_write(pHandle, buff, (leftSize >= mCapacity) ? mCapacity : leftSize, &error) < 0) {
                fprintf(stderr, "%s failed. %s\n", __FUNCTION__, pa_strerror(error));
                return false;
            }

            buff     += mCapacity;
            leftSize -= mCapacity;
        }

        return true;
    }

    return false;
}
