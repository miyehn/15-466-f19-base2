#pragma once
#include "GameObject.hpp"

struct Bullet : GameObject {

  Bullet(std::vector<Vertex> &vertices_, Level *lv_, glm::vec2 position_, glm::u8vec4 color_, float energy_, float speed) : 
    GameObject(vertices_, lv_, position_, color_) { energy = energy_; velocity = glm::vec2(speed*2, speed); }
  ~Bullet() override;

  // inherited
  void update(float elapsed, float min_x, float max_x) override;
  void draw_prep() override;

  // bullet-specific
  float energy = 0.0f;
  glm::vec2 velocity = glm::vec2(0, 0);
};
