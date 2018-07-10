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

#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <PmLogLib.h>

#define MSGID_LUNASERVICE   "GOOGLEAI_LUNASERVICE"

#define GOOGLEAI_LOG_CRITICAL(...) \
        PmLogCritical(getServiceContext(), MSGID_LUNASERVICE, 1, ##__VA_ARGS__)

#define GOOGLEAI_LOG_ERROR(...) \
        PmLogError(getServiceContext(), MSGID_LUNASERVICE, 0, ##__VA_ARGS__)

#define GOOGLEAI_LOG_WARNING(...) \
        PmLogWarning(getServiceContext(), MSGID_LUNASERVICE, 0, ##__VA_ARGS__)

#define GOOGLEAI_LOG_INFO(...) \
        PmLogInfo(getServiceContext(), MSGID_LUNASERVICE, 0, ##__VA_ARGS__)

#define GOOGLEAI_LOG_DEBUG(...) \
        PmLogDebug(getServiceContext(), ##__VA_ARGS__)

void logKmsg(const char *fmt, ...);
#define GOOGLEAI_KMSG_DEBUG_MSG(b, fmt, arg...)   if( (b) ) logKmsg(fmt, ##arg)

extern PmLogContext getServiceContext();

#endif
