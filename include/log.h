#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdbool.h>

#ifndef __LOG_ENABLE
#define __LOG_ENABLE false
#endif

#ifndef __LOG_DEBUG
#define __LOG_DEBUG false
#endif

#ifndef __LOG_FILE
#define __LOG_FILE false
#endif

extern FILE* __log_fp;

#define LOG_INIT(filename) \
    if(__LOG_ENABLE && __LOG_FILE) { \
        __log_fp = fopen(filename, "w"); \
    }

#define LOG_CLOSE(filename) \
    if(__LOG_ENABLE && __LOG_FILE) { \
        fclose(__log_fp); \
    }

#define LOG_INFO(fmt, args...) \
    if(__LOG_ENABLE) { \
        if(__LOG_FILE && __log_fp != NULL) \
            fprintf(__log_fp, "%s:%d - INFO: "fmt"\n", __FILE__, __LINE__, ##args); \
        else \
            fprintf(stdout, "%s:%d - INFO: "fmt"\n", __FILE__, __LINE__, ##args); \
    } 

#define LOG_DEBUG(fmt, args...) \
    if(__LOG_ENABLE && __LOG_DEBUG) { \
        if(__LOG_FILE && __log_fp != NULL) \
            fprintf(__log_fp, "%s:%d - DEBUG: "fmt"\n", __FILE__, __LINE__, ##args); \
        else \
            fprintf(stderr, "%s:%d - DEBUG: "fmt"\n", __FILE__, __LINE__, ##args); \
    }

#define LOG_ERROR(fmt, args...) \
    if(__LOG_ENABLE) { \
        if(__LOG_FILE && __log_fp != NULL) \
            fprintf(__log_fp, "%s:%d - ERROR: "fmt"\n", __FILE__, __LINE__, ##args); \
        else \
            fprintf(stderr, "%s:%d - ERROR: "fmt"\n", __FILE__, __LINE__, ##args); \
    }


#endif
