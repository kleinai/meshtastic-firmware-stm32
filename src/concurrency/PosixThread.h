/*
    Thread.h - An runnable object

    Thread is responsable for holding the "action" for something,
    also, it responds if it "should" or "should not" run, based on
    the current time;

    For instructions, go to https://github.com/ivanseidel/ArduinoThread

    Created by Ivan Seidel Gomes, March, 2013.
    Released into the public domain.
*/

#pragma once

#if defined(POSIX_THREAD)

#include <chrono>
#include <string>

static std::chrono::time_point<std::chrono::steady_clock> START_TIME = std::chrono::steady_clock::now();

static unsigned long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - START_TIME).count();
}

class Thread {
protected:
    // Desired interval between runs
    unsigned long interval;

    // Last runned time in Ms
    unsigned long last_run;

    // Scheduled run in Ms (MUST BE CACHED)
    unsigned long _cached_next_run;

    /*
        IMPORTANT! Run after all calls to run()
        Updates last_run and cache next run.
        NOTE: This MUST be called if extending
        this class and implementing run() method
    */
    void runned(unsigned long time);

    // Default is to mark it runned "now"
    void runned() { runned(millis()); }

    // Callback for run() if not implemented
    void (*_onRun)(void);

public:

    // If the current Thread is enabled or not
    bool enabled;

    // If true this thread wanting to run will not be used to prevent sleeping in runWithDelay
    bool canSleep = false;

    // ID of the Thread (initialized from memory adr.)
    size_t ThreadID;

#ifdef USE_THREAD_NAMES
    // Thread Name (used for better UI).
    std::string ThreadName;
#endif

    Thread(void (*callback)(void) = NULL, unsigned long _interval = 0);

    // Set the desired interval for calls, and update _cached_next_run
    virtual void setInterval(unsigned long _interval);

    // Return if the Thread should be runned or not
    // Note: no longer virtual - instead override tillRun
    bool shouldRun(unsigned long time);

    // Default is to check whether it should run "now"
    bool shouldRun() { return shouldRun(millis()); }

    // Return # of msecs till this thread will run again (or MAXINT if it is disabled).
    // If it is overdue the value will be negative
    virtual long tillRun(unsigned long time);

    // Callback set
    void onRun(void (*callback)(void));

    // Runs Thread
    virtual void run();
};

#endif