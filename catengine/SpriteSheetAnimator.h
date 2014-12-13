#ifndef SPRITE_SHEET_ANIMATOR_H
#define SPRITE_SHEET_ANIMATOR_H

#include "Types.h"
#include "Animator.h"
#include "SpriteSheetAnimation.h"

namespace catengine {
  class SpriteSheetAnimator : public Animator {
  public:
    SpriteSheetAnimator() = delete;
    SpriteSheetAnimator(SpriteSheetAnimation const& animation, bool carry_over_delta = false) :
      Animator(animation),
      duration_(0.0f),
      frame_(0),
      carry_over_(carry_over_delta)
    {
    }

    inline void reset()
    {
      duration_ = 0.0f;
      frame_ = 0;
    }

    State update(_decimal d_t);
    void draw(Renderer& renderer) const;

    inline _unsigned current_frame() const { return frame_; }

  private:
    _decimal duration_;
    _unsigned frame_;
    bool carry_over_;

    inline SpriteSheetAnimation::Frame const& current() const
    {
      return static_cast<SpriteSheetAnimation const*>(animation_)->frames_[frame_];
    }
  };
}

#endif