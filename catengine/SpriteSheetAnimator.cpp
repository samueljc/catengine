#include "SpriteSheetAnimator.h"

using namespace catengine;

SpriteSheetAnimator::State SpriteSheetAnimator::update(_decimal d_t)
{
  duration_ += d_t;
  if (duration_ >= current().duration) {
    duration_ = (carry_over_) ? duration_ - current().duration : 0.f;
    if (++frame_ >= static_cast<SpriteSheetAnimation const*>(animation_)->frames_.size()) {
      frame_ = 0;
      if (queued_animation_ != nullptr) {
        animation_ = queued_animation_;
        queued_animation_ = nullptr;
        return ANIMATION_SWAP;
      }
      return FINISHED;
    }
    return FRAME_SWAP;
  }
  return CONTINUE;
}

void SpriteSheetAnimator::draw(Renderer& renderer) const
{

}