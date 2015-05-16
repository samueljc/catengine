#include "Engine.hpp"
#include "Logging.hpp"

static void initialize_audio_test()
{
  ceng::Engine engine;

  engine.initialize_sound_buffer(48000, 2, 1);

  CHECK(engine.sound_enabled());
  CHECK_NNULL(engine.get_sound_buffer());
}

static int tests() 
{
  initialize_audio_test();

  return 0;
}