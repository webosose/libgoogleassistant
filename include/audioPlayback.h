#ifndef __AUDIOPLAYBACK_H__
#define __AUDIOPLAYBACK_H__

#include <pulse/simple.h>

class audioPlayback {
public:
    enum {
        PLAYBACK_CAPACITY_DEFAULT    = 1024,
        PLAYBACK_BITRATE_DEFAULT     = 16000,
        PLAYBACK_CHANNELS_DEFAULT    = 1
    };

public:
    audioPlayback( uint32_t capacity = PLAYBACK_CAPACITY_DEFAULT,
                   uint32_t rate = PLAYBACK_BITRATE_DEFAULT,
                   uint32_t channels = PLAYBACK_CHANNELS_DEFAULT );
    ~audioPlayback();

    bool        put(uint8_t *buff, size_t size);
    uint32_t    getCapacity() { return mCapacity; }
    uint32_t    getBitrate() { return mBitrate; }
    uint32_t    getChannels() { return mChannels; }

    void    flush();

private:
    void    initialize();
    void    finalize();

private:
    pa_simple   *pHandle;

    uint32_t    mCapacity;
    uint32_t    mBitrate;
    uint32_t    mChannels;
};

#endif
