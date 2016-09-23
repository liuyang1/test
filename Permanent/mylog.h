/**
 * @file mylog.h
 * @brief log module,devl by ZhJF,just modify
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-13
 */
#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#include <pthread.h>
#include <stdio.h>

// 全局变量,是否打开dump级别的信息输出
// #define _DUMP_INFO_	1

#define LOG_LEVEL_DEFAULT       LOG_NOTICE

typedef enum _my_log_level {
    LOG_MAX = 0,
    LOG_DEBUG,
    LOG_DUMP,
    LOG_INFO,
    LOG_NOTICE,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_NONE
} my_log_level;

// add {} to form code block to avoid some problem
#define INITLOG(x, y)    g_logger.init(x, y)
#define INFO(args ...)   {g_logger.log(LOG_INFO, ## args); g_logger.flush(); }
#define DEBUG(args ...)  {g_logger.log(LOG_DEBUG, ## args); g_logger.flush(); }
#define DUMP(args ...)   {g_logger.log(LOG_DUMP, ## args); g_logger.flush(); }
#define NOTICE(args ...) g_logger.log(LOG_NOTICE, ## args)
#define WARN(args ...)   g_logger.log(LOG_WARN, ## args)
#define ERROR(args ...)  g_logger.log(LOG_ERROR, ## args)

static char levelstr[][7] = {// not longer than 6 chars
    " MAX  ",
    "DEBUG ",
    " DUMP ",
    " INFO ",
    "NOTICE",
    " WARN ",
    "ERROR ",
    "FATAL ",
    " NONE "
};
class logger
{
    public:
        logger();
        ~logger();
        int init(const char *file, my_log_level level);
        int close();
        int log(my_log_level level, const char *fmt, ...);
        int rotate();
        //inline func
        void flush(void) {
            fflush(logfp);
        }

        static char *level2str(my_log_level level) {
            return levelstr[level];
        }

        static my_log_level str2level(const char *str);

        void locklog() {
            pthread_mutex_lock(&lock);
        }

        void unlocklog() {
            pthread_mutex_unlock(&lock);
        }

    private:
        FILE *logfp;
        char *logfile;
        my_log_level loglevel;
        pthread_mutex_t lock;
};
extern logger g_logger;


/*---------------------------------------------------------
 *  dump tool func
 *---------------------------------------------------------*/

void dumpBin(unsigned char *data, int len, int offset = 0);
inline void dumpBin(char *data, int len, int offset = 0) {
    dumpBin((unsigned char *)data, len, offset);
}

#endif
