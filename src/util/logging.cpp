/* @@@LICENSE
 *
 * Copyright (c) 2017 LG Electronics, Inc.
 *
 * Confidential computer software. Valid license from LG required for
 * possession, use or copying. Consistent with FAR 12.211 and 12.212,
 * Commercial Computer Software, Computer Software Documentation, and
 * Technical Data for Commercial Items are licensed to the U.S. Government
 * under vendor's standard commercial license.
 *
 * LICENSE@@@
 */

//=================================================================================================
// Includes & Definitions
//=================================================================================================

#include <stdio.h>
#include <glib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "logging.h"

//=================================================================================================
// Variables
//=================================================================================================


//=================================================================================================
// Functions
//=================================================================================================
void _good_assert(const char *cond_str, bool cond) {
    if (G_UNLIKELY(!(cond))) {
        GOOGLEAI_LOG_CRITICAL(PMLOGKS("CAUSE", cond_str), "");
        *(int *)0x00 = 0;
    }
}

PmLogContext getServiceContext() {
    static PmLogContext logContext = 0;

    if (0 == logContext) {
        PmLogGetContext("googleai", &logContext);
    }

    return logContext;
}

// For output to kernel message.
#define LOG_BUF_MAX 512
void logKmsg(const char *fmt, ...) {
    char buf[LOG_BUF_MAX];
    va_list ap;
    int log_fd;

    log_fd = open("/dev/kmsg", O_WRONLY);

    if (log_fd < 0) return;

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_MAX, fmt, ap);
    buf[LOG_BUF_MAX - 1] = 0;
    va_end(ap);

    write(log_fd, buf, strlen(buf));
    close(log_fd);
}
