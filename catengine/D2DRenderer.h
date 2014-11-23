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

    inline void draw_line(catengine::LineSegment const& line) const
    {
      render_target_->DrawLine({ line.start.x, line.start.y }, { line.end.x, line.end.y }, active_brush_.get(), thickness_);
    }

    inline void draw_rect(catengine::Rectangle const& rect) const
    {
      render_target_->DrawRectangle({ rect.x, rect.y, rect.x + rect.width, rect.y + rect.height }, active_brush_.get(), thickness_);
    }
    inline void fill_rect(catengine::Rectangle const& rect) const
    {
      render_target_->FillRectangle({ rect.x, rect.y, rect.x + rect.width, rect.y + rect.height }, active_brush_.get());
    }

    inline void draw_circle(catengine::Circle const& circle) const
    {
      render_target_->DrawEllipse({ { circle.x, circle.y }, circle.radius, circle.radius }, active_brush_.get(), thickness_);
    }
    inline void fill_circle(catengine::Circle const& circle) const
    {
      render_target_->FillEllipse({ { circle.x, circle.y }, circle.radius, circle.radius }, active_brush_.get());
    }

    void resize(_unsigned width, _unsigned height);

  private:
    ID2D1Factory* factory_;
    ID2D1HwndRenderTarget* render_target_;
    std::shared_ptr<ID2D1Brush> active_brush_;

    HRESULT create_render_target(HWND);
    void cleanup_render_target();

    // used by our smart pointer to know how to release the brush we feed into it
    struct ReleaseBrush {
      void operator()(ID2D1Brush* p) const {
        if (p != nullptr) {
          LOG(INFO) << "releasing";
          p->Release();
          p = nullptr;
        }
      }
    };
  };
}

#endif