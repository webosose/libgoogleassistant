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
