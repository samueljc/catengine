#ifndef D3D11_VERTEX_TYPES_H
#define D3D11_VERTEX_TYPES_H

#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

#include "Point2d.h"
#include "Point3d.h"
#include "Color.h"

using catengine::Point2d;
using catengine::Point3d;
using catengine::Color;

namespace catengine {
namespace D3D11 {
namespace VertexTypes {

// Vertex struct for position and color information.
struct PositionColor
{
  Point3d position;
  Color color;

  PositionColor()
  {
  }

  PositionColor(Point3d position, Color color) :
    position(position),
    color(color)
  {
  }

  static const int input_element_count = 2;
  static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};

// Vertex struct for position and texture information.
struct PositionTexture
{
  Point3d position;
  Point2d texture_coordinate;

  PositionTexture()
  {
  }

  PositionTexture(Point3d position, Point2d texture) :
    position(position),
    texture_coordinate(texture)
  {
  }

  static const int input_element_count = 2;
  static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};

// Vertex struct for position, color, and texture information.
struct PositionTextureColor
{
  Point3d position;
  Point2d texture_coordinate;
  Color color;

  PositionTextureColor()
  {
  }

  PositionTextureColor(Point3d position, Point2d texture, Color color) :
    position(position),
    texture_coordinate(texture),
    color(color)
  {
  }

  static const int input_element_count = 3;
  static const D3D11_INPUT_ELEMENT_DESC input_elements[input_element_count];
};
}
}
}

#endif