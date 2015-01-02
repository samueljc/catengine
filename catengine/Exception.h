#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace catengine {
class Exception : public std::exception {
public:
  Exception() :
    msg_(nullptr)
  {
  }

  Exception(char const* msg) :
    msg_(msg)
  {
  }

  Exception(std::string const& msg) :
    msg_(msg.c_str())
  {
  }

  virtual const char* what() const throw()
  {
    return msg_;
  }

  std::string what() const throw()
  {
    return std::string(msg_);
  }

private:
  char const* msg_;
};
}

#endif