#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Types.h"
#include "Renderer.h"
#include "Animation.h"

namespace catengine {
  class Animator {
  public:
    enum State {
      CONTINUE,
      FRAME_SWAP,
      ANIMATION_SWAP,
      FINISHED,
    };

    Animator(Animation const& animation) :
      animation_(&animation),
      queued_animation_(nullptr),
      updating_(true)
    {
    }

    virtual ~Animator() {}

    void start() { if (!updating_) updating_ = true; }
    void stop() { if (updating_) updating_ = false; }
    void restart() { reset(); start(); }

    void queue(Animation const& animation) { queued_animation_ = &animation; }
    void swap(Animation const& animation)
    {
      queued_animation_ = &animation;
      force_swap_ = true;
    }

    virtual void reset() = 0;

    virtual State update(_decimal d_t) = 0;
    virtual void draw(Renderer& renderer) const = 0;

  protected:
    Animation const* animation_;
    Animation const* queued_animation_;
    bool updating_;
    bool force_swap_;
  };
}

#endif