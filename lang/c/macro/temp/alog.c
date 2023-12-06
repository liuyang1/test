/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Log system for audio/Hifi part
 *
 * Author: Yang Liu <yang.liu@amlogic.com>
 * Version:
 * - 0.1        2019-05-23 init
 */
#include <stdarg.h>

#include "alog.h"
#if 0
#include "mailbox-api.h"
#include "aipc_type.h"
#include "rpc_srv.h"
#include "semphr.h"
#include "amx_msgQ.h"
#include "amx_task.h"
#endif

/** chip tag *****************************************************************/
// fixed length to 2
#if defined(HIFIA)
#define CHIP_TAG     "DA"
#elif defined(HIFIB)
#define CHIP_TAG     "DB"
#else
#define CHIP_TAG     "A " // for ARM
#endif

#ifdef __GNUC__
#define ALOG_INLINE     __attribute__((always_inline))
#else
#define ALOG_INLINE
#endif

// convert timestamp to string
// time is us based
#define TIME_STR_LEN        9

#define STR_LEN_MAX         128
#define SUFFIX_LEN          2 // '\n\0'
#define NUMBER 16
static uint8_t aLogLevelSt[MODULE_MAX] = {0};

static char* buffer[NUMBER][STR_LEN_MAX];
#if 0
static AMX_OSAL_MSGQ_HANDLE hEmptyQ = NULL;
static AMX_OSAL_MSGQ_HANDLE hFilledQ = NULL;
static AMX_OSAL_TASK_HANDLE hTaskAlog = NULL;
#endif
static bool aEnable = true;

static const struct {
    LogModule val;
    const char* const str;
} mapModuleStr[] = {
    { DUMMY, "DUMMY" },
    { AUTIL, "AUTIL" },
    { ASAMPLE, "ASAMPLE" },
    { MP3DEC, "MP3DEC" },
    { SPEEXENC, "SPEEXENC" },
    { TINYALSA, "TINYALSA" },
    { ADECODER, "ADECODER" },
    { FLATBUF, "FLATBUF" },
    { AMLPCM, "AMLPCM" }
};

static const struct {
    LogLevel val;
    const char* const str;
} mapLevelStr[] = {
    { LTRACE, "TRACE" },
    { LDEBUG, "DEBUG" },
    { LINFO, "INFO" },
    { LNOTICE, "NOTICE" },
    { LWARNING, "WARNING" },
    { LERROR, "ERROR" },
    { LFATAL, "FATAL" }
};

extern uint64_t xHwClockSourceRead(void);

uint64_t xHwClockSourceRead(void) {
    return 65531;
}

/**
 * @brief Log handler function type
 *
 * @param s input string
 *
 * @return length of output char
 *         -1, when fail to output log
 */
typedef int32_t (*LogHandler)(char *s);

// fixed length to 4
static inline ALOG_INLINE const char *showModule(LogModule m) {
    switch (m) {
        case DUMMY: return "DUM ";
        case AUTIL: return "UTIL";
        case ASAMPLE: return "SMPL";
        case MP3DEC: return "MP3D";
        case SPEEXENC: return "SPXE";
        case TINYALSA: return "ALSA";
        case FLATBUF: return "FLAT";
        case AMLPCM: return "TBUF";
        default: return "UNKM";
    }
}

// fixed length to one char
static inline ALOG_INLINE char showLevel(LogLevel l) {
    switch (l) {
        case LTRACE: return 'T';
        case LDEBUG: return 'D';
        case LINFO: return 'I';
        case LNOTICE: return 'N';
        case LWARNING: return 'W';
        case LERROR: return 'E';
        case LFATAL: return 'F';
        default: return 'U';
    }
}

static inline ALOG_INLINE LogModule readModule(char* s) {
    unsigned int i;
    for (i = 0; i < sizeof(mapModuleStr) / sizeof(*mapModuleStr); i++) {
        if (strcasecmp(s, mapModuleStr[i].str) == 0)
            return mapModuleStr[i].val;
    }
    return MODULE_MAX;
}

static inline ALOG_INLINE LogLevel readLevel(char *s) {
    unsigned int i;
    for (i = 0; i < sizeof(mapLevelStr) / sizeof(*mapLevelStr); i++) {
        if (strcasecmp(s, mapLevelStr[i].str) == 0)
            return mapLevelStr[i].val;
    }
    return LEVEL_MAX;
}

static inline ALOG_INLINE char *showTime(char *s, uint32_t maxLen, uint32_t t) {
    t /= 1000;
    uint32_t ms = t % 1000;
    t /= 1000;
    uint32_t sec = t;
    snprintf(s, maxLen, "%4d.%03d", sec, ms);
    return s;
}

static inline ALOG_INLINE int showLogHeader(char *s, uint32_t maxLen, LogModule m, LogLevel l,
                                            const char *func, int lineno) {
    char t_str[TIME_STR_LEN];
#if 0
    char *taskName = NULL;
    if (uxTaskGetNumberOfTasks() == 0) {
        taskName = (char *)"init";
    } else {
        TaskHandle_t task = xTaskGetCurrentTaskHandle();
        taskName = pcTaskGetName(task);
    }
#else
    char *taskName = "tsk";
#endif
    int n = snprintf(s, maxLen, "%8.8s %2.2s %4.4s %c %-8.8s %s():%d: ",
                    showTime(t_str, TIME_STR_LEN, (uint32_t)xHwClockSourceRead()),
                    CHIP_TAG, showModule(m), showLevel(l),
                    taskName, func, lineno);
    return n;
}

static inline ALOG_INLINE bool testLogLevel(LogModule m, LogLevel l) {
    // return aEnable && aLogLevelSt[m] & (1 << l);
    return 1;
}

static inline ALOG_INLINE int32_t console(char *s) {
    int32_t r;
    r = printf("%s", s);
    return r;
}

void setLogLevel(LogModule m, LogLevel l) {
    aLogLevelSt[m] = aLogLevelSt[m] | (1 << l);
}

void unsetLogLevel(LogModule m, LogLevel l) {
    aLogLevelSt[m] = aLogLevelSt[m] & (~(1 << l));
}

// it will enable log level from l to level max
void enableLogLevel(LogModule m, LogLevel l) {
    aLogLevelSt[m] = 0xFF ^ ((1 << l) - 1);
}

uint8_t getLogLevel(LogModule m) {
    return aLogLevelSt[m];
}

void enableAllModuleLevel(LogLevel l) {
    unsigned i;
    LogModule m;
    for (i = 0; i < sizeof(mapModuleStr) / sizeof(*mapModuleStr); i++) {
        m = mapModuleStr[i].val;
        enableLogLevel(m, l);
    }
}

void getAllModuleLevel() {
    unsigned int i, l;
    LogModule m;
    for (i = 0; i < sizeof(mapModuleStr) / sizeof(*mapModuleStr); i++) {
        m = mapModuleStr[i].val;
        l = __builtin_ctz(aLogLevelSt[m]);
        printf("%-8s %s\n",
               mapModuleStr[m].str, l < LEVEL_MAX ? mapLevelStr[l].str : "None");
    }
}

#if 0
static int32_t async_log(char *s) {
    char* msg;
    AMX_OSAL_MSGQ_GetMsg(hEmptyQ, &msg);
    strncpy(msg, s, STR_LEN_MAX);
    AMX_OSAL_MSGQ_PostMsg(hFilledQ, &msg);
    return 0;
}

static void* alog_task() {
    char* msg;
    while (1) {
        AMX_OSAL_MSGQ_GetMsg(hFilledQ, &msg);
        printf("%s", msg);
        AMX_OSAL_MSGQ_PostMsg(hEmptyQ, &msg);
    }
    return NULL;
}
#endif

static char* string_splice(LogModule m, LogLevel l, const char* func, int line, const char* format, va_list ap) {
    static char str[STR_LEN_MAX];
    int n = showLogHeader(str, STR_LEN_MAX, m, l, func, line);
    int ret = vsnprintf(str + n, STR_LEN_MAX - n, format, ap);
    if (ret >= STR_LEN_MAX - n - SUFFIX_LEN) {
        n = STR_LEN_MAX - SUFFIX_LEN;
    } else if (ret >= 0) {
        n += ret;
    }
    if (str[n - 1] != '\n') {
        n += snprintf(str + n, STR_LEN_MAX - n, "\n");
    }
    return str;
}

void alog_format(LogModule m, LogLevel l, const char* func, int line, const char* format, ...) {
    if (testLogLevel(m, l)) {
        char* p;
        va_list args;
        va_start(args, format);
        p = string_splice(m, l, func, line, format, args);
        va_end(args);
#if 0
        if (uxTaskGetNumberOfTasks() == 0 || hTaskAlog == NULL)
            console(p);
        else
            async_log(p);
#else
        console(p);
#endif
    }
}

void alog_format_fromISR(LogModule m, LogLevel l, const char* func, int line, const char* format, ...) {
    if (testLogLevel(m, l)) {
        char* p;
        va_list args;
        va_start(args, format);
        p = string_splice(m, l, func, line, format, args);
        va_end(args);
        console(p);
    }
}
void log_ctrl(int argc, char* argv[]) {
    if (strcasecmp(argv[0], "set") == 0) {
        if (argc == 3) {
            LogModule m = readModule(argv[1]);
            if (m == MODULE_MAX) {
                printf("Invalid parameter module\n");
                return;
            }
            LogLevel l = readLevel(argv[2]);
            if (l == LEVEL_MAX) {
                printf("Invalid parameter level\n");
                return;
            }
            printf("%-8s %s\n",
                   mapModuleStr[m].str, mapLevelStr[l].str);
            enableLogLevel(m, l);
        }
        if (argc == 2) {
            LogLevel l = readLevel(argv[1]);
            if (l == LEVEL_MAX) {
                printf("Invalid parameter level\n");
                return;
            }
            printf("%-8s\n", mapLevelStr[l].str);
            enableAllModuleLevel(l);
        }
    } else if (strcasecmp(argv[0], "get") == 0) {
        if (argc == 2) {
            LogModule m = readModule(argv[1]);
            if (m == MODULE_MAX) {
                printf("Invalid parameter module\n");
                return;
            }
            int l = __builtin_ctz(aLogLevelSt[m]);
            printf("%-8s %s\n",
                   mapModuleStr[m].str, l < LEVEL_MAX ? mapLevelStr[l].str : "None");
        }
        if (argc == 1) {
            getAllModuleLevel();
        }
    } else if (strcasecmp(argv[0], "enable") == 0) {
        aEnable = true;
    } else if (strcasecmp(argv[0], "disable") == 0) {
        aEnable = false;
    }
}

#if 0
void* log_handler_ctrl(void *arg)
{
    raw_st *p = (raw_st *)arg;
    int argc;
    char **argv;
    to_arg(p, &argc, &argv);
    log_ctrl(argc, argv);
    free_arg(argc, argv);
    return NULL;
}

int ALOG_Init() {
    unsigned int i = 0;
    xAIPCSrvInstallRemoteCB(MBX_CMD_LOG_CTRL, log_handler_ctrl, 1);
    AMX_OSAL_RET ret = AMX_OSAL_MSGQ_Create(&hEmptyQ, sizeof(char*), NUMBER);
    if (ret != AMX_OSAL_OK) {
        printf("Failed to create message Q\n");
        return -1;
    }
    ret = AMX_OSAL_MSGQ_Create(&hFilledQ, sizeof(char*), NUMBER);
    if (ret != AMX_OSAL_OK) {
        printf("Failed to create message Q\n");
        AMX_OSAL_MSGQ_Destroy(hEmptyQ);
        return -1;
    }
    ret = AMX_OSAL_TASK_Create(&hTaskAlog, alog_task, NULL, 0, "alog_task");
    if (ret != AMX_OSAL_OK) {
        printf("Failed to create task_msg_sender\n");
        AMX_OSAL_MSGQ_Destroy(hFilledQ);
        AMX_OSAL_MSGQ_Destroy(hEmptyQ);
        return -1;
    }

    for (i= 0; i< NUMBER; i++) {
        void *p = buffer + i;
        AMX_OSAL_MSGQ_PostMsg(hEmptyQ, &p);
    }
    return 0;
}
#endif
