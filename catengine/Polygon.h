#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <cstdarg>

#include "Types.h"
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

    Vector2& operator[](std::_unsigned i)
    {
      return vertices_[i];
    }
    Vector2 const& operator[](std::_unsigned i) const
    {
      return vertices_[i];
    }

    _unsigned size() const
    {
      return vertices_.size();
    }

    void clear()
    {
      vertices_.clear();
    }

    std::vector<Vector2>::const_iterator begin() const {
      return vertices_.cbegin();
    }
    std::vector<Vector2>::const_iterator end() const {
      return vertices_.cend();
    }

    void add(std::vector<Vector2> const& vertices)
    {
      vertices_.insert(vertices_.end(), vertices.cbegin(), vertices.cend());
    }
    void add(Vector2 const& vertex)
    {
      vertices_.push_back(vertex);
    }
    void remove(_integer i)
    {
      vertices_.erase(vertices_.begin() + i);
    }
    void insert(_integer i, Vector2 const& vertex)
    {
      vertices_.insert(vertices_.begin() + i, vertex);
    }
    void insert(_integer index, std::vector<Vector2> const& vertices)
    {
      vertices_.insert(vertices_.begin() + index, vertices.cbegin(), vertices.cend());
    }

  private:
    std::vector<Vector2> vertices_;
  };
}

#endif