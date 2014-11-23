#ifndef D2D_RENDERER_H
#define D2D_RENDERER_H

#include <Windows.h>
#include <d2d1.h>
#include <memory>
#include <iostream>

#pragma comment (lib, "d2d1.lib")

#include "Logging.h"
#include "Types.h"
#include "Renderer.h"
#include "Console.h"

namespace catengine {
  class D2DRenderer : public Renderer {
  public:
    D2DRenderer() :
      Renderer(),
      factory_(nullptr),
      render_target_(nullptr),
      active_brush_(nullptr)
    {
    }

    ~D2DRenderer();

    void initialize(HWND hwnd);
    void dispose();

    void begin_draw(HWND hwnd);
    void end_draw();

    void set_color(catengine::Color const& color);

    void draw_rect(catengine::Rectangle const& rect) const;
    void draw_line(catengine::LineSegment const& line) const;
    void draw_circle(catengine::Circle const& circle) const;

    void resize(_unsigned width, _unsigned height);

  private:
    ID2D1Factory* factory_;
    ID2D1HwndRenderTarget* render_target_;
    std::shared_ptr<ID2D1Brush> active_brush_;

    HRESULT create_render_target(HWND);
    void cleanup_render_target();

    struct ReleaseBrush {
      void operator()(ID2D1Brush* p) const {
        if (p != nullptr) {
          p->Release();
          p = nullptr;
        }
      }
    };
  };
}

#endif