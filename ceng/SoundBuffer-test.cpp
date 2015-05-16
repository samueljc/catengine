#include "SoundBuffer.hpp"
#include "Logging.hpp"

static void t0()
{
  ceng::SoundBuffer buffer(48000, 2, 1);
}

static int tests() 
{
  t0();

  return 0;
}