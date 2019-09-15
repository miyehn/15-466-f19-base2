#include "Bullet.hpp"

Bullet::~Bullet() {
}

void Bullet::update(float elapsed) {
  position += velocity * elapsed;
}

void Bullet::draw_prep() {
  rect(position, glm::vec2(2, 2), color);
}
