#ifndef LOGGER_H
#define LOGGER_H

enum LOG_LEVEL { ERROR = 0, DEBUG = 1, INFO = 2 };

typedef void (*log_fn_t)(const char *fmt, ...);

typedef struct {
  enum LOG_LEVEL log_level;
  log_fn_t info;
  log_fn_t debug;
  log_fn_t error;
} Logger;

void init_logger(Logger *logger);
void init_static_logger(void);

/* logger used by macros; defined once in logger.c */
extern Logger static_logger;

#define LOG_INFO(fmt, ...)                                                     \
  do {                                                                         \
    init_static_logger();                                                      \
    if (static_logger.info)                                                    \
      static_logger.info("%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);    \
  } while (0)

#define LOG_DEBUG(fmt, ...)                                                    \
  do {                                                                         \
    init_static_logger();                                                      \
    if (static_logger.log_level <= DEBUG && static_logger.debug)               \
      static_logger.debug("%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);   \
  } while (0)

#define LOG_ERROR(fmt, ...)                                                    \
  do {                                                                         \
    init_static_logger();                                                      \
    if (static_logger.error)                                                   \
      static_logger.error("%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);   \
  } while (0)

#endif /* LOGGER_H */
