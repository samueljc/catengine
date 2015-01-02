// SpriteSheetAnimation.h - Samuel Clark

#ifndef SPRITE_SHEET_ANIMATION_H
#define SPRITE_SHEET_ANIMATION_H

#include <memory>
#include <vector>

#include "Texture.h"
#include "Types.h"
#include "Rectangle.h"
#include "Point2d.h"
#include "Collider.h"
#include "Animation.h"

namespace catengine {
class SpriteSheetAnimation : public Animation {
public:
  struct Frame {
    Rectangle source;
    _decimal duration;
    Point2d origin;
    Collider* collider;

    Frame(Rectangle source, _decimal duration) :
      source(source),
      duration(duration),
      origin(source.center_x(), source.center_y()),
      collider(nullptr)
    {
    }

    Frame(Rectangle source, _decimal duration, Point2d origin) :
      source(source),
      duration(duration),
      origin(origin),
      collider(nullptr)
    {
    }
  };

  friend class SpriteSheetAnimator;

private:
  Texture* texture_;
  std::vector<Frame> frames_;
};
}

#endif