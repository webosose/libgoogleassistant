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
