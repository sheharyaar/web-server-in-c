#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define INFO 0
#define WARN 1
#define ERR 2

void ws_log(FILE* fp, int mode, char *fmt, ...);
void errExit(FILE *fp, char *str);

#endif
