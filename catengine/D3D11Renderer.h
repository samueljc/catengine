#ifndef D3D11_RENDERER_H
#define D3D11_RENDERER_H

#include <Windows.h>
#include <windowsx.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d2d1_1.h>
// may need d3dx10.h and d3dx10.lib

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d2d1.lib")

#include "IRenderer.h"
#include "D3D11VertexTypes.h"
#include "D3D11GeometryEffect.h"

#include <vector>
#include <string>

namespace catengine {
namespace D3D11 {

class Renderer : public IRenderer {
public:

  using GeometryVertex = VertexTypes::PositionColor;

  Renderer() :
    IRenderer(),
    stored_hwnd_(nullptr),
    initialized_(false),
    swap_chain_(nullptr),
    dev_(nullptr),
    devcon_(nullptr),
    dxgi_dev_(nullptr),
    dxgi_adapter_(nullptr),
    dxgi_factory_(nullptr),
    d2d_factory_(nullptr),
    d2d_dev_(nullptr),
    d2d_devcon_(nullptr),
    back_buffer_(nullptr),
    target_bitmap_(nullptr),
    depth_stencil_buffer_(nullptr),
    depth_stencil_state_(nullptr),
    depth_stencil_view_(nullptr),
    raster_state_(nullptr),
    sampler_state_(nullptr),
    max_geometry_vertices_(2048u),
    geometry_vertex_buffer_(nullptr),
    geometry_index_buffer_(nullptr),
    display_format_(DXGI_FORMAT_B8G8R8A8_UNORM),
    sample_count_(1),
    sample_quality_(D3D11_STANDARD_MULTISAMPLE_PATTERN),
    debug_(false),
    windowed_(true),
    refresh_numerator_(0),
    refresh_denominator_(1)
  {

  }

  RESULTS initialize(HWND hwnd);
  void dispose();
  void begin_draw();
  void end_draw();
  void draw_line(LineSegment const& line);
  void draw_rect(Rectangle const& rect);
  void fill_rect(Rectangle const& rect);
  void draw_circle(Circle const& rect);
  void fill_circle(Circle const& circle);
  void resize(_unsigned x, _unsigned y);
  void resize(HWND hwnd)
  {
    initialize_size_dependent_resources(hwnd);
  }

  LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
  {/*
    switch (message) {
    case WM_SIZE:
      initialize_size_dependent_resources(hwnd);
      break;
    }*/
  }

  /**
    * Number of supported quality levels for the given device. Must have the
    * d3d device initialized before calling this function.
    * @return The number of supported quality levels. 0 is an error.
    */
  UINT get_quality_levels();

  /**
    * Set the quality of our multi sampling to the given amount. If this
    * amount is not in the range of supported values then we do nothing.
    * @param quality The quality level we want to set to.
    */
  inline void set_sample_quality(UINT quality) {
    if (quality < get_quality_levels()) sample_quality_ = quality;
  }

private:
  void render_geometry();

  void device_lost();
  void initialize_size_dependent_resources(HWND hwnd);

  RESULTS collect_display_info(HWND const& hwnd);
  RESULTS collect_display_info(IDXGIFactory*& factory,
    IDXGIAdapter*& adapter,
    IDXGIOutput*& adapter_output,
    DXGI_MODE_DESC*& display_mode_list);

  void init_d3d_device();
  void init_dxgi_device();
  void init_d2d_device();
  void init_swap_chain(HWND hwnd);
  void init_back_buffer();
  void init_depth_buffer();
  void init_viewport();
  void init_rasterizer_state();
  void init_sampler_state();
  void init_blend_state();
  void create_blend_state(D3D11_BLEND src_blend,
    D3D11_BLEND dest_blend,
    D3D11_BLEND_OP blend_op,
    ID3D11BlendState** blend_state);
  void init_basic_shaders();
  void init_geometry_buffers();
  void init_resources();

private:
  HWND stored_hwnd_;
  bool initialized_;

  ID3D11Device* dev_;
  ID3D11DeviceContext* devcon_;
  
  IDXGIDevice1* dxgi_dev_;
  IDXGIAdapter* dxgi_adapter_;
  IDXGIFactory1* dxgi_factory_;

  ID2D1Factory1* d2d_factory_;
  ID2D1Device* d2d_dev_;
  ID2D1DeviceContext* d2d_devcon_;

  IDXGISwapChain* swap_chain_;
  ID3D11RenderTargetView* back_buffer_;
  ID2D1Bitmap1* target_bitmap_;

  ID3D11Texture2D* depth_stencil_buffer_;
  ID3D11DepthStencilState* depth_stencil_state_;
  ID3D11DepthStencilView* depth_stencil_view_;

  ID3D11RasterizerState* raster_state_;
  ID3D11SamplerState* sampler_state_;
  ID3D11BlendState* blend_state_;

  D3D11_VIEWPORT viewport_;

  UINT max_geometry_vertices_;
  UINT max_geometry_indices_;
  std::vector<GeometryVertex> geometry_vertices_staged_;
  std::vector<short> geometry_indices_staged_;
  ID3D11Buffer* geometry_vertex_buffer_;
  ID3D11Buffer* geometry_index_buffer_;

  GeometryEffect geometry_effect_;

  DXGI_FORMAT display_format_;
  UINT sample_count_;
  UINT sample_quality_;
  bool debug_;
  bool windowed_;
  UINT refresh_numerator_;
  UINT refresh_denominator_;

  static const D3D_FEATURE_LEVEL feature_levels_[];
  static const UINT feature_levels_size_;
};

inline void throw_if_failed(HRESULT hr, std::string msg)
{
  if (hr != S_OK) {
    _com_error err(hr);
    // todo: append the com error to the message
    throw std::exception(msg.c_str());
  }
}
}
}

#endif