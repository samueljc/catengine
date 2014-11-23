#ifndef LOGGING_H
#define LOGGING_H

#include <cassert>
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
      switch (severity_) {
      case Severity::INFO:
        msg_ << "I ";
        break;
      case Severity::WARNING:
        msg_ << "W ";
        break;
      case Severity::ERROR:
        msg_ << "E ";
        break;
      case Severity::FATAL:
        msg_ << "F ";
        break;
      }

      auto now = std::chrono::high_resolution_clock::now();
      time_t time = std::chrono::system_clock::to_time_t(now);

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

      msg_ << "[" << time_str << " ";
      msg_ << GetCurrentProcessId();
      msg_ << " " << file_name << ":" << static_cast<unsigned>(line) << "] ";
    }

    virtual ~Message()
    {
      // todo: trace backtrace here

      msg_ << std::endl;
      if (severity_ == Severity::INFO || severity_ == Severity::WARNING)
        std::cout << msg_.str();
      else
        std::cerr << msg_.str();

      if (severity_ == Severity::FATAL) {
        abort();
      }
    }

    inline std::ostream& stream() { return msg_; }

  private:
    Severity severity_;
    std::ostringstream msg_;
  };
}

#endif