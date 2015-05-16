#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <string.h>

#ifdef ERROR
#undef ERROR
#endif

#define SIMPLE

#define LOG_SUCCESS clog::Message(__FILE__, __LINE__, clog::Severity::SUCCESS)
#define LOG_INFO    clog::Message(__FILE__, __LINE__, clog::Severity::INFO)
#define LOG_WARNING clog::Message(__FILE__, __LINE__, clog::Severity::WARNING)
#define LOG_ERROR   clog::Message(__FILE__, __LINE__, clog::Severity::ERROR)
#define LOG_FATAL   clog::Message(__FILE__, __LINE__, clog::Severity::FATAL)

#define LOG(severity) LOG_##severity.stream()

#define CHECK(expr1) if (!(expr1)) \
  LOG(FATAL)

#define CHECK_NULL(expr1) if ((expr1) != nullptr) \
  LOG(FATAL) << "CHECK_NULL(" << (expr1) << ") failed";

#define CHECK_NNULL(expr1) if ((expr1) == nullptr) \
  LOG(FATAL) << "CHECK_NNULL(" << (expr1) << ") failed";

#define CHECK_EQ(expr1, expr2) if ((expr1) != (expr2)) \
  LOG(FATAL) << "CHECK_EQ(" << (expr1) << ", " << (expr2) << ") failed";

#define CHECK_NE(expr1, expr2) if ((expr1) == (expr2)) \
  LOG(FATAL) << "CHECK_NE(" << (expr1) << ", " << (expr2) << ") failed";

#define CHECK_LT(expr1, expr2) if ((expr1) <= (expr2)) \
  LOG(FATAL) << "CHECK_LT(" << (expr1) << ", " << (expr2) << ") failed";

#define CHECK_LE(expr1, expr2) if ((expr1) < (expr2)) \
  LOG(FATAL) << "CHECK_LE(" << (expr1) << ", " << (expr2) << ") failed";

#define CHECK_GT(expr1, expr2) if ((expr1) >= (expr2)) \
  LOG(FATAL) << "CHECK_GT(" << (expr1) << ", " << (expr2) << ") failed";

#define CHECK_GE(expr1, expr2) if ((expr1) > (expr2)) \
  LOG(FATAL) << "CHECK_GE(" << (expr1) << ", " << (expr2) << ") failed";


namespace clog {
enum class Severity {
  SUCCESS,
  INFO,
  WARNING,
  ERROR,
  FATAL
};

class Message {
public:
  Message(char const* file, int line) :
    Message(file, line, Severity::INFO)
  {

  }
  Message(char const* file, int line, Severity severity) :
    severity_(severity)
  {
    info_ << "[";
    switch (severity_) {
    case Severity::SUCCESS:
      info_ << "S ";
      break;
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

#ifndef SIMPLE
    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    // need higher precision
    auto whole_second = std::chrono::system_clock::from_time_t(time);
    auto microsecond = std::chrono::duration_cast<std::chrono::microseconds>(now - whole_second).count();

    char const* time_fmt = "%H:%M:%S";
    // char const* time_fmt = "%Y-%m-%d %H:%M:%S";

    info_ << std::put_time(std::localtime(&time), time_fmt);
    info_ << "." << std::setfill('0') << std::setw(6) << microsecond << " ";
    info_ << GetCurrentProcessId();
    info_ << ":";
    info_ << GetCurrentThreadId() << " ";
#endif

    // todo: let's just show the filename instead of the absolute path
    info_ << file << ":" << static_cast<unsigned>(line) << "]";
  }

  virtual ~Message()
  {
    msg_ << std::endl;

    std::cout << info_.str();
    std::cout << " " << msg_.str();

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

#endif // LOGGING_HPP
