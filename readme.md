# CHIP-8

## 1
    GLuint texture;
    {
        int width = 32;
        int height = 32;
        unsigned char pixels[32 * 32 * 4];

        for (int i = 0; i < width * height; ++i)
        {
            pixels[i * 4 + 0] = 255; // Red
            pixels[i * 4 + 1] = 0;   // Green
            pixels[i * 4 + 2] = 0;   // Blue
            pixels[i * 4 + 3] = 255; // Alpha
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    #if defined(GL_UNPACK_ROW_LENGTH)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    #endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }

## 2
        // Write code here..
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    ImVec2 canvasViewportStart(100, 100); // Position where to draw

    draw_list->AddImage((ImTextureID)texture,
            canvasViewportStart,
            ImVec2(canvasViewportStart.x + 60, canvasViewportStart.y + 32));

## 3
    glDeleteTextures(1, &texture);


// SDL
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
const int height = 320, width = 640;

SDL_Init(SDL_INIT_EVERYTHING);

window = SDL_CreateWindow("emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
renderer = SDL_CreateRenderer(window, -1, 0);
SDL_RenderSetLogicalSize(renderer, width, height);
texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

// Write code here..
uint32_t pixels[32 * 64];

for (int i = 0; i <= 32 * 64; i++) {
    auto& pixel = pixels[i]; 
    if (i % 2 == 0) {
        pixel = 0xFF000000;
    } else {
        pixel = 0xFFFFFFFF;
    }
}

SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
SDL_RenderClear(renderer);
SDL_RenderCopy(renderer, texture, NULL, NULL);
SDL_RenderPresent(renderer);

usleep(500000000);

SDL_DestroyTexture(texture);