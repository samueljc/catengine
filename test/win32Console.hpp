#ifndef WIN32_CONSOLE_HPP
#define WIN32_CONSOLE_HPP

#include <windows.h>
#include <io.h>
#include <iostream>
#include <sstream>
#include <memory>

// todo: make this not rely on stdio
class win32Console {
public:
  static void open() {
    if (console_ == nullptr) console_ = new win32Console();
  }

  /**
    * Dispose / close the console. This doesn't actually switch stdout/stderr
    * back to wherever we got them from, so if that's what you're trying to do
    * then you shouldn't have created the console to begin with.
    */
  static void close() {
    FreeConsole();
    if (console_ != nullptr) delete console_;
    if (pcout_ != nullptr) fclose(pcout_);
    if (pcerr_ != nullptr) fclose(pcerr_);
  }
    
private:
  static win32Console* console_;
  static FILE* pcout_;
  static FILE* pcerr_;

  win32Console() {
    AllocConsole();

    freopen_s(&pcout_, "CONOUT$", "w", stdout);
    freopen_s(&pcerr_, "CONERR$", "w", stderr);

    std::ios::sync_with_stdio();
  }

  win32Console(win32Console&& c) = delete;
  win32Console(win32Console const& c) = delete;
};

#endif // WIN32_CONSOLE_HPP