#pragma once
#include "GameObject.hpp"

struct Bullet : GameObject {

  Bullet(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_, float energy_) : 
    GameObject(vertices_, position_, color_) { energy = energy_; }
  ~Bullet() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

  // bullet-specific
  bool active = true;
  float energy = 0.0f;
  glm::vec2 velocity = glm::vec2(40, 20);
};
