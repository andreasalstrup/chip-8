#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#else
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#endif

namespace platform {
constexpr int WIDTH = 64;
constexpr int HEIGHT = 32;
constexpr int DISPLAY_SIE = WIDTH * HEIGHT;
constexpr int SCALE = 20;

enum class action {
  off,
  on,
  flip,
};

class Bitmap {
  uint32_t *pixels;
  GLuint texture;

public:
  Bitmap(uint32_t *pixels);
  ~Bitmap();
  void display();
  void update();
  uint32_t getPixel(int x, int y);
  int setPixel(int pos, action action);
  int setPixel(int x, int y, action action);
};
} // namespace platform
