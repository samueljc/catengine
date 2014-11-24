#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <string>
#include <memory>

#include "Types.h"
#include "Rectangle.h"
#include "Vector2d.h"
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

    inline Vector2d position() const
    {
      return position_;
    }
    inline void position(Vector2d const& v)
    {
      position_ = v;
      update_transformation();
    }

    inline Vector2d rotation() const
    {
      return rotation_;
    }
    inline void rotation(Vector2d const& v)
    {
      rotation_ = v;
      update_transformation();
    }

    inline Vector2d scale() const
    {
      return scale_;
    }
    inline void scale(Vector2d const& v)
    {
      scale_ = v;
      update_transformation();
    }

    inline Vector2d velocity() const
    {
      return velocity_;
    }
    inline void velocity(Vector2d const& v)
    {
      velocity_ = v;
    }

    inline Vector2d acceleration() const
    {
      return acceleration_;
    }
    inline void acceleration(Vector2d const& v)
    {
      acceleration_ = v;
    }

    inline bool remove_from_quadtree() const {
      return true;
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

    static Rectangle get_bounding_rect(GameObject const& c)
    {
      return Rectangle(c.position_.x, c.position_.y, 5.f, 5.f);
    }

  private:
    Vector2d position_;
    Vector2d rotation_;
    Vector2d scale_;
    Matrix2 transformation_;
    Vector2d velocity_;
    Vector2d acceleration_;

    std::vector<std::string> tags_;

    void update_transformation();
  };

  typedef std::shared_ptr<GameObject> GameObjectPtr;
}

#endif