#include "Throttle.h"

#ifndef EXCLUDE_ARDUINO
#include <Arduino.h>
#else
#include <chrono>
static std::chrono::time_point<std::chrono::steady_clock> START_TIME = std::chrono::steady_clock::now();

static unsigned long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - START_TIME).count();
}
#endif

/// @brief Execute a function throttled to a minimum interval
/// @param lastExecutionMs Pointer to the last execution time in milliseconds
/// @param minumumIntervalMs Minimum execution interval in milliseconds
/// @param throttleFunc Function to execute if the execution is not deferred
/// @param onDefer Default to NULL, execute the function if the execution is deferred
/// @return true if the function was executed, false if it was deferred
bool Throttle::execute(uint32_t *lastExecutionMs, uint32_t minumumIntervalMs, void (*throttleFunc)(void), void (*onDefer)(void))
{
    if (*lastExecutionMs == 0) {
        *lastExecutionMs = millis();
        throttleFunc();
        return true;
    }
    uint32_t now = millis();

    if ((now - *lastExecutionMs) >= minumumIntervalMs) {
        throttleFunc();
        *lastExecutionMs = now;
        return true;
    } else if (onDefer != NULL) {
        onDefer();
    }
    return false;
}

/// @brief Check if the last execution time is within the interval
/// @param lastExecutionMs The last execution time in milliseconds
/// @param timeSpanMs The interval in milliseconds of the timespan
bool Throttle::isWithinTimespanMs(uint32_t lastExecutionMs, uint32_t timeSpanMs)
{
    return (millis() - lastExecutionMs) < timeSpanMs;
}