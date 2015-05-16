#include "win32Console.hpp"

win32Console* win32Console::console_ = nullptr;
FILE* win32Console::pcout_ = nullptr;
FILE* win32Console::pcerr_ = nullptr;