#include "Obstacle.hpp"

Obstacle::~Obstacle() {
}

void Obstacle::update(float elapsed) {

}

void Obstacle::draw_prep() {
  rect(position, glm::vec2(10, 10), color);
}

