#include "Obstacle.hpp"

Obstacle::~Obstacle() {
}

void Obstacle::update(float elapsed, float min_x, float max_x) {
  GameObject::update(elapsed, min_x, max_x);
  if (position.x < min_x) dead = true;
}

void Obstacle::draw_prep() {
  rect(position, glm::vec2(4, 4), color);
}

