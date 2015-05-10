#include <stdafx.h>

#include "Logging.h"
#include "Console.h"

#define internal static
#define local_persist static
#define global static

global bool running;

global BITMAPINFO bitmap_info;
global void* bitmap_memory;
global HBITMAP backbuffer;
global HDC bitmap_devcon;

internal void win32_resize_DIB_section(int width, int height)
{
  if (backbuffer != nullptr) {
    DeleteObject(backbuffer);
    backbuffer = nullptr;
  } 
  if (bitmap_devcon == nullptr) {
    // TODO: maybe need to recreate this... check if we do
    bitmap_devcon = CreateCompatibleDC(0);
  }

  bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth = width;
  bitmap_info.bmiHeader.biHeight = height;
  bitmap_info.bmiHeader.biPlanes = 1;
  bitmap_info.bmiHeader.biBitCount = 32;
  bitmap_info.bmiHeader.biCompression = BI_RGB;

  backbuffer = CreateDIBSection(bitmap_devcon,
    &bitmap_info,
    DIB_RGB_COLORS,
    &bitmap_memory,
    nullptr,
    0);
}

internal void win32_update_window(HDC devcon, int x, int y, int width, int height)
{
  StretchDIBits(devcon,
    x, y, width, height,
    x, y, width, height,
    bitmap_memory,
    &bitmap_info,
    DIB_RGB_COLORS,
    SRCCOPY);
}

LRESULT CALLBACK main_window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
  LRESULT result(0);
  
  switch (message) {
  case WM_SIZE:
  {
    RECT client_rect;
    GetClientRect(window, &client_rect);
    win32_resize_DIB_section(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
  } break;
  case WM_DESTROY:
    running = false;
    break;
  case WM_CLOSE:
    running = false;
    break;
  case WM_ACTIVATEAPP:
    LOG(INFO) << "WM_ACTIVATEAPP";
    break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    auto devcon = BeginPaint(window, &ps);
    win32_update_window(devcon, 
      ps.rcPaint.left,
      ps.rcPaint.top,
      ps.rcPaint.right - ps.rcPaint.left, 
      ps.rcPaint.bottom - ps.rcPaint.top);
    EndPaint(window, &ps);
  } break;
  default:
    result = DefWindowProc(window, message, w_param, l_param);
  }

  return result;
}

int APIENTRY _tWinMain(HINSTANCE instance,
  HINSTANCE prev_instance,
  LPTSTR command_line,
  int show_command)
{
  WNDCLASS window_class = {};

  window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  window_class.lpfnWndProc = main_window_callback;
  window_class.hInstance = instance;
  window_class.lpszClassName = L"test";

  catengine::Console::open();
  
  if (!RegisterClass(&window_class)) {
    // TODO: get last error, blah blah blah
    LOG(FATAL) << "could not register window class";
  }

  HWND window = CreateWindowEx(
    0,
    window_class.lpszClassName,
    L"CATch",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    instance,
    0);
  if (!window) {
    // TODO: get last error, blah blah blah
    LOG(FATAL) << "could not create window";
  }
    
  MSG message;
  running = true;
  while (running) {
    auto result = GetMessage(&message, nullptr, 0, 0);
    if (result > 0) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    } else if (result == 0) {
      break;
    } else {
      LOG(ERROR) << "bad result: " << result;
      break;
    }
  }
}