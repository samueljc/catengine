#ifndef LOGGING_H
#define LOGGING_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#undef ERROR
#endif

#define LOG_INFO    catengine::Message(__FILE__, __LINE__)
#define LOG_WARNING catengine::Message(__FILE__, __LINE__, catengine::Severity::WARNING)
#define LOG_ERROR   catengine::Message(__FILE__, __LINE__, catengine::Severity::ERROR)
#define LOG_FATAL   catengine::Message(__FILE__, __LINE__, catengine::Severity::FATAL)

#define LOG(severity) LOG_##severity.stream()

namespace catengine {
  enum class Severity {
    INFO,
    WARNING,
    ERROR,
    FATAL
  };

  class Message {
  public:
    Message(char const* file, int line, std::string const& msg) :
      Message(file, line, Severity::FATAL)
    {
      msg_ << msg;
    }
    Message(char const* file, int line) :
      Message(file, line, Severity::INFO)
    {

    }
    Message(char const* file, int line, Severity severity) :
      severity_(severity)
    {
      info_ << "[";
      switch (severity_) {
      case Severity::INFO:
        info_ << "I ";
        break;
      case Severity::WARNING:
        info_ << "W ";
        break;
      case Severity::ERROR:
        info_ << "E ";
        break;
      case Severity::FATAL:
        info_ << "F ";
        break;
      }

      auto now = std::chrono::high_resolution_clock::now();
      time_t time = std::chrono::system_clock::to_time_t(now);
      // need higher precision
      auto whole_second = std::chrono::system_clock::from_time_t(time);
      auto microsecond = std::chrono::duration_cast<std::chrono::microseconds>(now - whole_second).count();

      std::tm time_info;
#ifdef _WIN32
      if (localtime_s(&time_info, &time) != 0) {
        // failed to get the local time
      }
#endif

      // todo: something better here eventually when i know what i want to do
#ifdef _DEBUG
      char time_str[10];
      char const* time_fmt = "%H:%M:%S";
#else
      char time_str[20];
      char const* time_fmt = "%Y-%m-%d %H:%M:%S";
#endif
      if (strftime(time_str, sizeof(time_str), time_fmt, &time_info) == 0) {
        // did not manage to copy everything into the array
      }

      // let's just show the filename instead of the absolute path
      const char* file_name = strrchr(file, '\\') + 1;
      if (file_name == nullptr) file_name = file;

      info_ << time_str << "." << std::setfill('0') << std::setw(6) << microsecond << " ";
      info_ << GetCurrentProcessId();
      info_ << ":";
      info_ << GetCurrentThreadId();
      info_ << " " << file_name << ":" << static_cast<unsigned>(line) << "]";
    }

    virtual ~Message()
    {
      msg_ << std::endl;

#if _WIN32
      HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(hstdout, &csbi);

      switch (severity_) {
      case Severity::INFO:
        SetConsoleTextAttribute(hstdout,
          FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
      case Severity::WARNING:
        SetConsoleTextAttribute(hstdout, 
          FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
      case Severity::ERROR:
        SetConsoleTextAttribute(hstdout, 
          FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
      case Severity::FATAL:
        SetConsoleTextAttribute(hstdout, 
          FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
        break;
      }

      std::cout << info_.str();
      SetConsoleTextAttribute(hstdout, csbi.wAttributes);
      std::cout << " " << msg_.str();
#else
      std::cout << info_.str();
      std::cout << " " << msg_.str();
#endif

      if (severity_ == Severity::FATAL) {
        abort();
      }
    }

    inline std::ostream& stream() { return msg_; }

  private:
    Severity severity_;
    std::ostringstream info_;
    std::ostringstream msg_;
  };
}

#endif