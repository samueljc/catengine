#include "Console.h"

catengine::Console* catengine::Console::console_ = nullptr;
FILE* catengine::Console::pcout_ = nullptr;
FILE* catengine::Console::pcerr_ = nullptr;