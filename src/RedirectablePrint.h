#pragma once

#include <iostream>

#include "../freertosinc.h"
#include "mesh/generated/meshtastic/mesh.pb.h"
#include <stdarg.h>
#include <string>


#if !defined(POSIX_PRINT)
#include <Print.h>
#define PRINT_DRIVER Print
#else

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#include <iostream>
#include <cstdio>

class PosixPrint {
private:
  int write_error;

protected:
  void setWriteError(int err = 1) {
    write_error = err;
  }

public:
  PosixPrint() : write_error(0) {}

  int getWriteError() { return write_error; }

  void clearWriteError() { write_error = 0; }

  size_t write(uint8_t c) { return printf("%c", c); }

  size_t write(const char *str) { return printf("%s", str); }

  size_t write(const uint8_t *buffer, size_t size) {
    std::string str = std::string((const char*) buffer, size);
    if (write_error == 1)
      std::cerr << str;
    else
      std::cout << str;
    return str.length();
  }

  size_t write(const char *buffer, size_t size) {
    return write((const uint8_t *)buffer, size);
  }

  virtual int availableForWrite() {
    return 0;
  }

  // size_t print(const __FlashStringHelper *);
  // size_t print(const String &);
  size_t print(const char str[]) { return printf("%s", str); }
  size_t print(char c) { return printf("%c", c); }
  size_t print(unsigned char, int = DEC);
  size_t print(int, int = DEC);
  size_t print(unsigned int, int = DEC);
  size_t print(long, int = DEC);
  size_t print(unsigned long, int = DEC);
  size_t print(long long, int = DEC);
  size_t print(unsigned long long, int = DEC);
  size_t print(float, int = 2);
  size_t print(double, int = 2);
  // size_t print(const Printable &);

  // size_t println(const __FlashStringHelper *);
  // size_t println(const String &s);
  size_t println(const char[]);
  size_t println(char);
  size_t println(unsigned char, int = DEC);
  size_t println(int, int = DEC);
  size_t println(unsigned int, int = DEC);
  size_t println(long, int = DEC);
  size_t println(unsigned long, int = DEC);
  size_t println(long long, int = DEC);
  size_t println(unsigned long long, int = DEC);
  size_t println(float, int = 2);
  size_t println(double, int = 2);
  // size_t println(const Printable &);
  size_t println(void);

  int printf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int len = vprintf(format, ap);
    va_end(ap);
    return len;
  }

  int vprintf(const char *format, va_list ap) {
    char buf[1024];
    int len = std::vsnprintf(buf, 1023, format, ap);
    if (write_error == 1)
      std::cerr << buf;
    else
      std::cout << buf;
    return len;
  }

  virtual void flush() {

  }
};

#define PRINT_DRIVER PosixPrint
#endif

/**
 * A Printable that can be switched to squirt its bytes to a different sink.
 * This class is mostly useful to allow debug printing to be redirected away from Serial
 * to some other transport if we switch Serial usage (on the fly) to some other purpose.
 */
class RedirectablePrint : public PRINT_DRIVER
{
    PRINT_DRIVER *dest;

#ifdef HAS_FREE_RTOS
    SemaphoreHandle_t inDebugPrint = nullptr;
    StaticSemaphore_t _MutexStorageSpace;
#else
    volatile bool inDebugPrint = false;
#endif
  public:
    explicit RedirectablePrint(PRINT_DRIVER *_dest) : dest(_dest) {}

    /**
     * Set a new destination
     */
    void rpInit();
    void setDestination(PRINT_DRIVER *dest);

    virtual size_t write(uint8_t c);

    /**
     * Debug logging print message
     *
     * If the provide format string ends with a newline we assume it is the final print of a single
     * log message.  Otherwise we assume more prints will come before the log message ends.  This
     * allows you to call logDebug a few times to build up a single log message line if you wish.
     */
    void log(const char *logLevel, const char *format, ...) __attribute__((format(printf, 3, 4)));

    /** like printf but va_list based */
    size_t vprintf(const char *logLevel, const char *format, va_list arg);

    void hexDump(const char *logLevel, unsigned char *buf, uint16_t len);

    std::string mt_sprintf(const std::string fmt_str, ...);

  protected:
    /// Subclasses can override if they need to change how we format over the serial port
    virtual void log_to_serial(const char *logLevel, const char *format, va_list arg);
    meshtastic_LogRecord_Level getLogLevel(const char *logLevel);

  private:
    void log_to_syslog(const char *logLevel, const char *format, va_list arg);
    void log_to_ble(const char *logLevel, const char *format, va_list arg);
};