#include <stdio.h>
#include <unistd.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

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


#ifdef _WIN32
    #include <windows.h>        // SetProcessDPIAware()
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../imgui/examples/libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include "app.hpp"
#include "../graphics/frame_buffer.hpp"

namespace chip8
{

    int display_framebuffer(const FrameBuffer& frame_buffer) {
        auto texture = frame_buffer.get();
        ImDrawList* bg = ImGui::GetBackgroundDrawList();
        bg->AddImage(
            (ImTextureID)(intptr_t)texture,
            ImVec2(0,0),
            ImVec2(ImVec2(640, 320))
        );
        return 0;
    }

    int RenderUI() {
        // SDL window + OpenGL
        SDL_Window* window;
        SDL_GLContext gl_context;
        const int height = 320, width = 640;

        SDL_Init(SDL_INIT_VIDEO);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        window = SDL_CreateWindow("emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Init framebuffer
        uint32_t pixels[32 * 64];
        auto framebuffer = FrameBuffer{pixels};

        bool running = true;
        SDL_Event event;
        while (running) {
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                    running = false;
            }

            // Display init
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            // Display render
            display_framebuffer(framebuffer);

            ImGui::Begin("Debug Window");
            ImGui::Text("Hello, Emu!");

            if (ImGui::Button("Click me!")) {
                for (auto i = 0; i <= 32 * 64; i++) {
                    auto& pixel = pixels[i];
                    if (i % 2 == 0) {
                        pixel = 0xFFFFFFFF;
                    } else {
                        pixel = 0xFF000000;
                    }
                }
                framebuffer.update(pixels);
            }

            ImGui::End();

            // Display draw
            ImGui::Render();
            glViewport(0, 0, width, height);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            SDL_GL_SwapWindow(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 0;
    }
}