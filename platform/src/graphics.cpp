#include "graphics.hpp"
#include "imgui.h"
#include <iostream>
#include <ostream>

namespace platform {
Bitmap::Bitmap(uint32_t *pixels) : pixels{pixels} {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  for (int i = 0; i < DISPLAY_SIE; ++i) {
    auto &pixel = pixels[i];
    pixel = (i % 2 == 0) ? 0xFF000000 : 0xFFFFFFFF;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Bitmap::~Bitmap() { glDeleteTextures(1, &texture); }

void Bitmap::display() {
  auto texture = this->texture;
  ImDrawList *bg = ImGui::GetBackgroundDrawList();
  bg->AddImage((ImTextureID)(intptr_t)texture, ImVec2(0, 0),
               ImVec2(ImVec2(WIDTH * SCALE, HEIGHT * SCALE)));
}

void Bitmap::update() {
  glBindTexture(GL_TEXTURE_2D, this->texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGBA,
                  GL_UNSIGNED_BYTE, this->pixels);
}

uint32_t Bitmap::getPixel(int x, int y) {
  int pos = y * WIDTH + x;
  return this->pixels[pos];
}

int Bitmap::setPixel(int pos, action action) {
  if (this->pixels == NULL) {
    std::cerr << "[ERROR] Bitmap:setPixel error: failed to get pixels."
              << std::endl;
    return 1;
  }

  auto &pixel = this->pixels[pos];

  switch (action) {
  case action::off:
    pixel = 0xFFFFFFFF;
    break;
  case action::on:
    pixel = 0xFF000000;
    break;
  case action::flip:
    pixel = (pixel == 0xFFFFFFFF) ? 0xFF000000 : 0xFFFFFFFF;
    break;
  default:
    break;
  }

  return 0;
}

int Bitmap::setPixel(int x, int y, action action) {
  int pos = y * WIDTH + x;
  return this->setPixel(pos, action);
}
} // namespace platform
