#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
enum LOG_LEVEL { ERROR = 0, INFO = 1, DEBUG = 2 };

typedef void (*log_fn_t)(const char *fmt, ...);

typedef struct {
  enum LOG_LEVEL log_level;
  int print_location;
  FILE *info_output;
  FILE *debug_output;
  FILE *error_output;
} Logger;

void log_info(FILE *output, const char *fmt, ...);
void log_debug(FILE *output, const char *fmt, ...);
void log_error(FILE *output, const char *fmt, ...);

// Warning: this logger is not thread safe!
Logger *get_static_logger(void);

void init_logger(Logger *logger);
void init_static_logger(void);
void set_log_level(Logger *logger, enum LOG_LEVEL log_level);

#define LOG_DEBUG(logger, fmt, ...)                                            \
  if ((logger)->log_level >= DEBUG) {                                          \
    if ((logger)->print_location == 1) {                                       \
      log_debug((logger)->debug_output, "%s:%d: " fmt, __FILE__, __LINE__,     \
                ##__VA_ARGS__);                                                \
    } else {                                                                   \
      log_debug((logger)->debug_output, fmt, ##__VA_ARGS__);                   \
    }                                                                          \
  }

#define LOG_INFO(logger, fmt, ...)                                             \
  if ((logger)->log_level >= INFO) {                                           \
    if ((logger)->print_location == 1) {                                       \
      log_info((logger)->info_output, "%s:%d: " fmt, __FILE__, __LINE__,       \
               ##__VA_ARGS__);                                                 \
    } else {                                                                   \
      log_info((logger)->info_output, fmt, ##__VA_ARGS__);                     \
    }                                                                          \
  }

#define LOG_ERROR(logger, fmt, ...)                                            \
  if ((logger)->log_level >= ERROR) {                                          \
    if ((logger)->print_location == 1) {                                       \
      log_error((logger)->error_output, "%s:%d: " fmt, __FILE__, __LINE__,     \
                ##__VA_ARGS__);                                                \
    } else {                                                                   \
      log_error((logger)->error_output, fmt, ##__VA_ARGS__);                   \
    }                                                                          \
  }

#endif /* LOGGER_H */
