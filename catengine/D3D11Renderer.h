#ifndef D3D11_RENDERER_H
#define D3D11_RENDERER_H

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

#include "Renderer.h"

namespace catengine {
  class D3D11Renderer : Renderer {
  public:
    D3D11Renderer() :
      Renderer(),
      swp_chain_(nullptr),
      dev_(nullptr),
      dev_context_(nullptr),
      texture_format_(DXGI_FORMAT_R8G8B8A8_UNORM),
      sample_count_(0),
      sample_quality_(0),
      debug_(false),
      fullscreen_(false)
    {

    }

    /**
     * Calls everything in order to initialize this renderer. If this has not 
     * been called then we cannot use this object to render anything. This
     * method is not automatically called by the constructor and you should
     * specify any special settings you want before making a call to this so
     * that they can be applied.
     */
    void initialize(HWND hwnd);

    /**
     * Disposes all of our lingering com objects.
     */
    void dispose();

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
    IDXGISwapChain* swp_chain_;
    ID3D11Device* dev_;
    ID3D11DeviceContext* dev_context_;
    ID3D11RenderTargetView* backbuffer_;
    D3D11_VIEWPORT viewport_;

    DXGI_FORMAT texture_format_;
    UINT sample_count_;
    UINT sample_quality_;
    bool debug_;
    bool fullscreen_;

    void init_swap_chain_and_device(HWND hwnd);
    void init_backbuffer();
    void init_viewport();
  };
}

#endif