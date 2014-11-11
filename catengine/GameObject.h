#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <string>

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
    GameObject() :
      scale_(1.0f, 1.0f)
    {
      tags_.reserve(10);
    }

    inline Vector2 position() const
    {
      return position_;
    }
    inline void position(Vector2 const& v)
    {
      position_ = v;
      update_transformation();
    }

    inline Vector2 rotation() const
    {
      return rotation_;
    }
    inline void rotation(Vector2 const& v)
    {
      rotation_ = v;
      update_transformation();
    }

    inline Vector2 scale() const
    {
      return scale_;
    }
    inline void scale(Vector2 const& v)
    {
      scale_ = v;
      update_transformation();
    }

    inline Vector2 velocity() const
    {
      return velocity_;
    }
    inline void velocity(Vector2 const& v)
    {
      velocity_ = v;
    }

    inline Vector2 acceleration() const
    {
      return acceleration_;
    }
    inline void acceleration(Vector2 const& v)
    {
      acceleration_ = v;
    }

    /**
     * Check to see if this object is tagged.
     * @param Tag we're looking for.
     * @return true if found, false otherwise
     */
    bool tagged(std::string const& tag) const
    {
      for (auto const& t : tags_) {
        if (t == tag) {
          return true;
        }
      }
      return false;
    }

    void add_tag(std::string const& tag);
    void remove_tag(std::string const& tag);

  private:
    Vector2 position_;
    Vector2 rotation_;
    Vector2 scale_;
    Matrix2 transformation_;
    Vector2 velocity_;
    Vector2 acceleration_;

    std::vector<std::string> tags_;

    void update_transformation();
  };
}

#endif