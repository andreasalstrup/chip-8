#include <vector>
#include "imgui.h"

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

using texture_t = GLuint;

namespace chip8
{
    class FrameBuffer {
        uint32_t* pixels;
        GLuint texture;
       
        public:

        explicit FrameBuffer(uint32_t* pixels): pixels{pixels} {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            for (auto i = 0; i <= 32 * 64; i++) {
                auto& pixel = pixels[i];
                if (i % 2 == 0) {
                    pixel = 0xFF000000;
                } else {
                    pixel = 0xFFFFFFFF;
                }
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        ~FrameBuffer() {
            glDeleteTextures(1, &texture);
        }

        void update(uint32_t* pixels) {
            glBindTexture(GL_TEXTURE_2D, this->texture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }

        const texture_t get() const {
            return this->texture;
        }
    };
}