#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <cstdarg>

#include "Types.h"
#include "Point2d.h"

namespace catengine {
  class Polygon {
  public:
    Polygon() :
      vertices_()
    {
    }

    Polygon(std::vector<Point2d> const& vertices) :
      vertices_(vertices)
    {
    }

    inline Point2d& operator[](_unsigned i)
    {
      return vertices_[i];
    }
    inline Point2d const& operator[](_unsigned i) const
    {
      return vertices_[i];
    }

    inline _unsigned size() const
    {
      return vertices_.size();
    }

    inline void clear()
    {
      vertices_.clear();
    }

    inline std::vector<Point2d>::const_iterator begin() const {
      return vertices_.cbegin();
    }
    inline std::vector<Point2d>::const_iterator end() const {
      return vertices_.cend();
    }

    inline void add(std::vector<Point2d> const& vertices)
    {
      vertices_.insert(vertices_.end(), vertices.cbegin(), vertices.cend());
    }
    inline void add(Point2d const& vertex)
    {
      vertices_.push_back(vertex);
    }

    inline void remove(_integer i)
    {
      if (static_cast<size_t>(i) < vertices_.size()) 
        vertices_.erase(vertices_.begin() + i);
    }

    inline void insert(_integer i, Point2d const& vertex)
    {
      vertices_.insert(vertices_.begin() + i, vertex);
    }
    inline void insert(_integer index, std::vector<Point2d> const& vertices)
    {
      vertices_.insert(vertices_.begin() + index, vertices.cbegin(), vertices.cend());
    }

  private:
    std::vector<Point2d> vertices_;
  };
}

#endif