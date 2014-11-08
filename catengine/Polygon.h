#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <cstdarg>

#include "Vector2.h"

namespace catengine {
  class Polygon {
  public:
    Polygon() :
      vertices_()
    {
    }

    Polygon(std::vector<Vector2> const& vertices) :
      vertices_(vertices)
    {
    }

    Vector2& operator[](std::size_t i)
    {
      return vertices_[i];
    }
    Vector2 const& operator[](std::size_t i) const
    {
      return vertices_[i];
    }

    void add_vertex(Vector2 const& vertex)
    {
      vertices_.push_back(vertex);
    }
    void remove_vertex(int i)
    {
      vertices_.erase(vertices_.begin() + i);
    }
    void insert_vertex(int i, Vector2 const& vertex)
    {
      vertices_.insert(vertices_.begin() + i, vertex);
    }

  private:
    std::vector<Vector2> vertices_;
  };
}

#endif