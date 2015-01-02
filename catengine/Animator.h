// Animator.hpp - Samuel Clark
//
// Virtual methods:
//   void reset();
//   State update(_decimal);
//   void draw(Renderer&);
//

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Types.h"
#include "Renderer.h"
#include "Animation.h"

namespace catengine {
class Animator {
public:
  // enum that describes the state of an animation after an update cycle
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

  inline void start() { if (!updating_) updating_ = true; }
  inline void stop() { if (updating_) updating_ = false; }
  inline void restart() { reset(); start(); }

  inline void queue(Animation const& animation) { queued_animation_ = &animation; }
  inline void swap(Animation const& animation)
  {
    queued_animation_ = &animation;
    force_swap_ = true;
  }

  virtual void reset() = 0;

  virtual State update(_decimal d_t) = 0;
  virtual void draw(Renderer& renderer) const = 0;

protected:
  void const* animation_;
  void const* queued_animation_;
  bool updating_;
  bool force_swap_;
};
}

#endif