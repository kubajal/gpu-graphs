#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

// Warning: this logger is not thread safe!
Logger static_logger;

static void vlog_impl(FILE *out, const char *prefix,
                      const char *fmt, va_list ap)
{
    fputs(prefix, out);
    vfprintf(out, fmt, ap);
    fputc('\n', out);
}

static void log_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_impl(stdout, "[INFO]  ", fmt, ap);
    va_end(ap);
}

static void log_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_impl(stdout, "[DEBUG] ", fmt, ap);
    va_end(ap);
}

static void log_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_impl(stderr, "[ERROR] ", fmt, ap);
    va_end(ap);
}

void init_logger(Logger *logger)
{
    if (!logger) return;
    logger->log_level = INFO;
    logger->info = log_info;
    logger->debug = log_debug;
    logger->error = log_error;
}

// Warning: this logger is not thread safe!
void init_static_logger(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    init_logger(&static_logger);
    LOG_INFO("Static logger initialized");
}
