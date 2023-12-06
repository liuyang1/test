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

#ifndef _ALOG_H_
#define _ALOG_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if 0
#include "FreeRTOS.h"
#include "task.h"

#include "aosal.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** module *******************************************************************/
typedef enum {
    DUMMY = 0,
    AUTIL,
    ASAMPLE,
    MP3DEC,
    SPEEXENC,
    TINYALSA,
    ADECODER,
    FLATBUF,
    AMLPCM,
    MODULE_MAX,
} LogModule;

/** log level ****************************************************************/
typedef enum {
    LTRACE = 0x00,
    LDEBUG = 0x01,
    LINFO = 0x02,
    LNOTICE = 0x03,
    LWARNING = 0x04,
    LERROR = 0x05,
    LFATAL = 0x06,
    LEVEL_MAX = 0x7,
} LogLevel;

#define ALOG(module, level, ...) alog_format(module, level, __func__, __LINE__, __VA_ARGS__)
#define ALOGFROMISR(module, level, ...) alog_format_fromISR(module, level, __func__, __LINE__, __VA_ARGS__)

void setLogLevel(LogModule m, LogLevel l);
void unsetLogLevel(LogModule m, LogLevel l);
void enableLogLevel(LogModule m, LogLevel l);
uint8_t getLogLevel(LogModule m);

void alog_format(LogModule m, LogLevel l, const char* func, int line, const char* format, ...);
void alog_format_fromISR(LogModule m, LogLevel l, const char* func, int line, const char* format, ...);

int ALOG_Init(void);

#ifdef __cplusplus
}
#endif
#endif
