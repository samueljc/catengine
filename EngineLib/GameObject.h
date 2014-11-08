#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector2.h"
#include "Matrix2.h"

namespace catengine {
  /**
   * The core class of our game engine. 
   * This is the base class for our component model and every basic game object
   * should be derived from it.
   */
  class GameObject {
  public:

  private:
    Vector2 position_;
    Vector2 rotation_;
    Vector2 scale_;
    Matrix2 transformation_;
    Vector2 velocity_;
    Vector2 acceleration_;
  };
}

#endif