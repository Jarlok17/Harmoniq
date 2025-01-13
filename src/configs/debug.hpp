#pragma once

#include <QDebug>
#include <iostream>
#include <qassert.h>

#define HAR_ON 1
#define HAR_OFF 0

#define HAR_DEBUGING HAR_ON

void logParams();

#if HAR_DEBUGING
#define LOG_INFO(msg) qDebug() << "[INFO]:" << msg
#define LOG_WARNING(msg) qWarning() << "[WARNING]:" << msg
#define LOG_ERROR(msg) qCritical() << "[ERROR]:" << msg
#define LOG_SUCCESS(msg) qDebug() << "\033[32m[SUCCESS]:\033[0m" << msg
#define LOG_DEBUG(msg) qDebug() << "[DEBUG]:" << msg
#define LOG(...) logParams(__VA_ARGS__)
#else
#define LOG_INFO(msg)
#define LOG_WARNING(msg)
#define LOG_ERROR(msg)
#define LOG_SUCCESS(msg)
#define LOG_DEBUG(msg)
#endif

inline void har_assert(const char *assertion, const char *file, int line)
{
    qCritical() << "Assertion failed: " << assertion << "in file" << file << "at line" << line;
    assert(false);
}

inline void logParams() { std::cout << std::endl; }

inline void logParams(const char *param1) { std::cout << param1 << std::endl; }

template <typename T, typename... Args> inline void logParams(const T &first, const Args &...rest)
{
    std::cout << first << " ";
    logParams(rest...);
}

#if HAR_DEBUGING
#define HAR_ASSERT(cond)                                                                                                            \
    if ((!cond)) {                                                                                                                  \
        har_assert(#cond, __FILE__, __LINE__);                                                                                      \
    }
#else
#define HAR_ASSERT(cond)
#endif
