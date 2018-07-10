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
