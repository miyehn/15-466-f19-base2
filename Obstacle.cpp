#include "Obstacle.hpp"
#include <iostream>

Obstacle::Obstacle(std::vector<Vertex> &vertices_, glm::vec2 position_) : GameObject(vertices_) {
  vertices = &vertices_;
  position = position_;
}

Obstacle::~Obstacle() {}

void Obstacle::update(float elapsed) {

}

void Obstacle::draw_prep() {
  if (!vertices) { std::cout << "Obstacle: vertices is null" << std::endl; }
  rect(position, glm::vec2(10, 10), color);
}

