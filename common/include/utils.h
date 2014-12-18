#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include <stddef.h>



#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

#define ULOG(level, format, ...) do {\
    time_t now = time(NULL);\
    char timestr[20];\
    strftime(timestr, 20, TIME_FORMAT, localtime(&now));\
    fprintf(stderr, "\e[01;32m %s " level ": \e[0m" format "\n", timestr, ##__VA_ARGS__);\
    }\
    while(0)

#define  LOGD(format, ...) do {\
    ULOG("INFO", format, ##__VA_ARGS__);\
    }\
    while(0)

#define  LOGE(format, ...) do {\
    ULOG("ERROR", format, ##__VA_ARGS__);\
    }\
    while(0)

/* #define offsetof(type, member) ({\ */
/*     ((size_t)( &( ((type *)0)->member ) )) */
/* }) */

#define CONTAINER_OF(ptr, type, member) ({\
    const __typeof__( ((type *)0)->member) *__mptr = ptr; \
    (type *)( (char *)__mptr - offsetof(type, member)); })


#endif /* UTILS_H */
