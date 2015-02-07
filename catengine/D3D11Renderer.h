#ifndef D3D11_RENDERER_H
#define D3D11_RENDERER_H

#include <Windows.h>
#include <windowsx.h>
#include <dxgi.h>
#include <d3d11.h>
// may need d3dx10.h and d3dx10.lib

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")

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
    swap_chain_(nullptr),
    dev_(nullptr),
    dev_context_(nullptr),
    back_buffer_(nullptr),
    depth_stencil_buffer_(nullptr),
    depth_stencil_state_(nullptr),
    depth_stencil_view_(nullptr),
    raster_state_(nullptr),
    sampler_state_(nullptr),
    max_geometry_vertices_(2048u),
    geometry_vertex_buffer_(nullptr),
    geometry_index_buffer_(nullptr),
    display_format_(DXGI_FORMAT_R8G8B8A8_UNORM),
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
  RESULTS swap_shader_state();
  void begin_draw();
  void end_draw();
  void draw_line(LineSegment const& line);
  void draw_rect(Rectangle const& rect);
  void fill_rect(Rectangle const& rect);
  void draw_circle(Circle const& rect);
  void fill_circle(Circle const& circle);
  void resize(_unsigned x, _unsigned y);

  LRESULT CALLBACK WindowProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);

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

  RESULTS collect_display_info(HWND const& hwnd);
  RESULTS collect_display_info(IDXGIFactory*& factory,
    IDXGIAdapter*& adapter,
    IDXGIOutput*& adapter_output,
    DXGI_MODE_DESC*& display_mode_list);

  RESULTS init_swap_chain_and_device(HWND hwnd);
  RESULTS init_back_buffer();
  RESULTS init_depth_buffer();
  RESULTS init_rasterizer_state();
  RESULTS init_sampler_state();
  RESULTS init_blend_state();
  RESULTS create_blend_state(D3D11_BLEND src_blend,
    D3D11_BLEND dest_blend,
    D3D11_BLEND_OP blend_op,
    ID3D11BlendState** blend_state);
  RESULTS init_viewport();
  RESULTS init_basic_shaders();
  RESULTS init_geometry_buffers();
  RESULTS init_resources();

private:
  HWND stored_hwnd_;
  IDXGISwapChain* swap_chain_;
  ID3D11Device* dev_;
  ID3D11DeviceContext* dev_context_;
  ID3D11RenderTargetView* back_buffer_;
  ID3D11Texture2D* depth_stencil_buffer_;
  ID3D11DepthStencilState* depth_stencil_state_;
  ID3D11DepthStencilView* depth_stencil_view_;
  ID3D11RasterizerState* raster_state_;
  ID3D11SamplerState* sampler_state_;
  // todo: multiple blend states
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
};
}
}

#endif