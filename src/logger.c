#include "logger.h"
#include "graph.h"

#include <stdarg.h>
#include <stdio.h>

// Warning: this logger is not thread safe!
static Logger static_logger;

Logger* get_static_logger(void)
{
    return &static_logger;
}

static void vlog_impl(FILE *out, const char *prefix,
                      const char *fmt, va_list ap)
{
    fputs(prefix, out);
    vfprintf(out, fmt, ap);
    fputc('\n', out);
}

void log_info(FILE* output, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_impl(output, "[INFO]  ", fmt, ap);
    va_end(ap);
}

void log_debug(FILE* output, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_impl(output, "[DEBUG] ", fmt, ap);
    va_end(ap);
}

void log_error(FILE* output, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_impl(output, "[ERROR] ", fmt, ap);
    va_end(ap);
}

void set_log_level(Logger *logger, enum LOG_LEVEL log_level)
{
    logger->log_level = log_level;
}

void init_logger(Logger *logger)
{
    if (!logger) return;
    logger->print_location = 1;
    logger->log_level = INFO;
    logger->info_output = stdout;
    logger->debug_output = stdout;
    logger->error_output = stderr;
    LOG_INFO(logger, "Logger initialized with INFO log level");
}

void print_graph(const Graph *graph, unsigned current, unsigned indent) {
  printf("%*c%u\n", indent, ' ', current);
  for (int i = 0; i < graph->edges_n; i++) {
    const Edge* edge = &graph->edges[i];
    printf("%u %u", edge->source, edge->target);
  }
}
