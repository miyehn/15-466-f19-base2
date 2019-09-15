#include "Bullet.hpp"

Bullet::~Bullet() {
}

void Bullet::update(float elapsed, float min_x, float max_x) {
  GameObject::update(elapsed, min_x, max_x);
  position += velocity * elapsed;
  if (position.x > max_x) dead = true;
}

void Bullet::draw_prep() {
  rect(position, glm::vec2(2, 2), color);
}
