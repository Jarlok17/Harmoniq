#pragma once

namespace harmoniq {
#if defined(_WIN32) || defined(_WIN64)
#define HAR_OS_WINDOWS
#elif defined(__linux__)
#define HAR_OS_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define HAR_OS_MAC
#elif define(__ANDROID__)
#define HAR_OS_ANDROID
#endif // defined(_WIN32) || defined(_WIN64)
} // namespace harmoniq
