#include "D2DRenderer.h"

using namespace catengine;

D2DRenderer::~D2DRenderer()
{
  dispose();
}

void D2DRenderer::initialize(HWND hwnd) 
{
  HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);
  if (hr != S_OK) {
    // log an error
    dispose();
    return;
  }

  FLOAT x, y;
  factory_->GetDesktopDpi(&x, &y);
  dpi_x_ = static_cast<_decimal>(x);
  dpi_y_ = static_cast<_decimal>(y);

  create_render_target(hwnd);
}

void D2DRenderer::dispose()
{
  cleanup_render_target();
  if (factory_ != nullptr) {
    factory_->Release();
    factory_ = nullptr;
  }
}

void D2DRenderer::begin_draw(HWND hwnd)
{
  LOG(INFO) << "begin draw";
  HRESULT hr = create_render_target(hwnd);

  if (hr == S_OK) {
    render_target_->BeginDraw();
    render_target_->SetTransform(D2D1::Matrix3x2F::Identity());
    render_target_->Clear(D2D1::ColorF(D2D1::ColorF::White));

    set_color(catengine::Color::BLACK);
  }
  if (hr == D2DERR_RECREATE_TARGET) {
    cleanup_render_target();
  }
}

void D2DRenderer::end_draw()
{
  render_target_->EndDraw();
}

void D2DRenderer::set_color(catengine::Color const& color)
{
  if (render_target_ == nullptr) {
    LOG(ERROR) << "fart";
    return;
  }
  D2D1_COLOR_F d2d_color;

  ID2D1SolidColorBrush* brush;
  render_target_->CreateSolidColorBrush(d2d_color, &brush);
  active_brush_.reset(brush, ReleaseBrush());
}

void D2DRenderer::draw_rect(catengine::Rectangle const& rect) const
{
  render_target_->DrawRectangle({ rect.x, rect.y, rect.width, rect.height }, active_brush_.get());
}

void D2DRenderer::draw_line(catengine::LineSegment const& line) const
{

}

void D2DRenderer::draw_circle(catengine::Circle const& circle) const
{

}

void D2DRenderer::resize(_unsigned width, _unsigned height)
{
  // nothing
}

HRESULT D2DRenderer::create_render_target(HWND hwnd)
{
  if (factory_ == nullptr) {
    // log an error
    return D2DERR_WRONG_FACTORY;
  }
  if (render_target_ == nullptr) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size{ rc.right - rc.left, rc.bottom - rc.top };

    HRESULT hr = factory_->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(hwnd, size),
      &render_target_);

    if (hr != S_OK) {
      // todo: log an error
      return hr;;
    }
  }
  return S_OK;
}

void D2DRenderer::cleanup_render_target()
{
  if (render_target_ != nullptr) {
    render_target_->Release();
    render_target_ = nullptr;
  }
}