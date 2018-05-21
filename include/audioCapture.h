#ifndef __AUDIOCAPTURE_H__
#define __AUDIOCAPTURE_H__

#include <pulse/simple.h>

class audioCapture {
public:
    enum {
        CAPTURE_CAPACITY_DEFAULT    = 1024,
        CAPTURE_BITRATE_DEFAULT     = 16000,
        CAPTURE_CHANNELS_DEFAULT    = 1
    };

public:
    audioCapture( uint32_t capacity = CAPTURE_CAPACITY_DEFAULT,
                  uint32_t rate = CAPTURE_BITRATE_DEFAULT,
                  uint32_t channels = CAPTURE_CHANNELS_DEFAULT );
    ~audioCapture();

    uint8_t*    get(size_t size = 0);
    uint32_t    getCapacity() { return mCapacity; }
    uint32_t    getBitrate() { return mBitrate; }
    uint32_t    getChannels() { return mChannels; }

    void    flush();

private:
    void    initialize();
    void    finalize();

private:
    pa_simple   *pHandle;
    uint8_t     *pBuff;

    uint32_t    mCapacity;
    uint32_t    mBitrate;
    uint32_t    mChannels;
};

#endif
