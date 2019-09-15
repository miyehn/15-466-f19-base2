#include "Bullet.hpp"

Bullet::~Bullet() {
}

void Bullet::update(float elapsed) {
  if (active) position += velocity * elapsed;
}

void Bullet::draw_prep() {
  if (active) rect(position, glm::vec2(2, 2), color);
}
