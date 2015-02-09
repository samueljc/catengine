// Renderer.h - Samuel Clark
//
// Virtual methods:
//   void initialize(HWND)
//   void dispose()
//   void begin_draw()
//   void end_draw()
//   void set_color(Color const&)
//   void draw_line(LineSegment const&) const
//   void draw_rect(Rectangle const&) const
//   void fill_rect(Rectangle const&) const
//   void draw_circle(Circle const&) const
//   void fill_circle(Circle const&) const
//   void resize(_unsigned, _unsigned)

#ifndef IRENDERER_H
#define IRENDERER_H

#include "Types.h"
#include "Rectangle.h"
#include "LineSegment.h"
#include "Circle.h"
#include "Color.h"
#include "Texture.h"
#include "Results.h"
#include "DisplayDesc.h"

namespace catengine {

class IRenderer {
public:
  /* todo: Make texture part of the renderer.
    * Different renderers rely on different texture formats, so I think this
    * makes sense.
    */

  IRenderer() :
    dpi_x_(96.0f),
    dpi_y_(96.0f),
    canvas_width_(0),
    canvas_height_(0),
    vsync_enabled_(false),
    thickness_(1.0f),
    half_thickness_(0.5f)
  {
  }
  virtual ~IRenderer() = default;
  virtual RESULTS initialize(HWND h_wnd) = 0;

  /* This may be worth implementing
  virtual void initialize_device_independent_resources();
  virtual void initialize_device_dependent_resources(HWND h_wnd);
  */
  virtual void dispose() = 0;

  virtual void begin_draw() = 0;
  virtual void end_draw() = 0;

  virtual void draw_line(LineSegment const& line) = 0;

  virtual void draw_rect(Rectangle const& rectangle) = 0;
  virtual void fill_rect(Rectangle const& rectangle) = 0;

  virtual void draw_circle(Circle const& circle) = 0;
  virtual void fill_circle(Circle const& circle) = 0;

  virtual void resize(_unsigned width, _unsigned height) = 0;

  //LRESULT CALLBACK WindowProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);

  inline _decimal get_dpi_x() const { return dpi_x_; }
  inline _decimal get_dpi_y() const { return dpi_y_; }


  inline void set_color(Color color) { brush_color_ = color; }
  inline Color get_color() const { return brush_color_; }

  inline void set_thickness(_decimal thickness) 
  {
    if (thickness <= 0.0f) return;
    thickness_ = thickness;
    half_thickness_ = thickness_ / 2.0f;
  }
  inline _decimal get_thickness() const { return thickness_; }

protected:
  DisplayDesc display_desc_;

  _decimal dpi_x_;
  _decimal dpi_y_;
  _unsigned canvas_width_;
  _unsigned canvas_height_;
  _unsigned vsync_enabled_;

  Color brush_color_;
  _decimal thickness_;
  _decimal half_thickness_;
};
}

#endif