#include "D2DRenderer.h"

using namespace catengine;

D2DRenderer::~D2DRenderer()
{
  dispose();
}

void D2DRenderer::initialize(HWND hwnd) 
{
  if (factory_ == nullptr) {
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);
    if (!SUCCEEDED(hr)) {
      LOG(ERROR) << "could not create factory";
      dispose();
      return;
    }

    FLOAT x, y;
    factory_->GetDesktopDpi(&x, &y);
    dpi_x_ = static_cast<_decimal>(x);
    dpi_y_ = static_cast<_decimal>(y);
  }

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

void D2DRenderer::begin_draw()
{
  render_target_->BeginDraw();
  render_target_->SetTransform(D2D1::Matrix3x2F::Identity());
  render_target_->Clear(D2D1::ColorF(D2D1::ColorF::White));
  render_target_->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

  // reset our values for a fresh drawing cycle
  thickness_ = 1.0f;
  set_color(catengine::Color::BLACK);
}

void D2DRenderer::end_draw()
{
  render_target_->EndDraw();
}

void D2DRenderer::set_color(catengine::Color const& color)
{
  if (render_target_ == nullptr) {
    LOG(ERROR) << "Render target is null.";
    return;
  }
  D2D1_COLOR_F d2d_color{color.r(), color.g(), color.b(), color.a()};

  ID2D1SolidColorBrush* brush;
  render_target_->CreateSolidColorBrush(d2d_color, &brush);
  active_brush_.reset(brush, ReleaseBrush());
}

void D2DRenderer::resize(_unsigned width, _unsigned height)
{
  // nothing
}

HRESULT D2DRenderer::create_render_target(HWND hwnd)
{
  if (factory_ == nullptr) {
    LOG(ERROR) << "no factory created";
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
      LOG(ERROR) << "problem creating render target: " << hr;
      return hr;
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