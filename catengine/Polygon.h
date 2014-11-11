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

    size_t size() const
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
    void remove(int i)
    {
      vertices_.erase(vertices_.begin() + i);
    }
    void insert(int i, Vector2 const& vertex)
    {
      vertices_.insert(vertices_.begin() + i, vertex);
    }
    void insert(int index, std::vector<Vector2> const& vertices)
    {
      vertices_.insert(vertices_.begin() + index, vertices.cbegin(), vertices.cend());
    }

  private:
    std::vector<Vector2> vertices_;
  };
}

#endif