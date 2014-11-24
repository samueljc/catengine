#ifndef TRACER_H
#define TRACER_H

#include <string>

#include "Logging.h"

namespace catengine {
  class Tracer {
  public:
    Tracer(std::string trace) :
      trace_(trace)
    {
      LOG(INFO) << trace_ << " entered";
    }

    virtual ~Tracer() {
      LOG(INFO) << trace_ << " exited";
    }

  private:
    std::string trace_;
  };
}

#endif
