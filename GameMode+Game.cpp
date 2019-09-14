#include "GameMode.hpp"
#include "Obstacle.cpp"

void GameMode::init() {
  GameObject* tmp1 = new Obstacle(vertices, glm::vec2(100, 0));
  GameObject* tmp2 = new Obstacle(vertices, glm::vec2(300, 0));
  objects.push_back(tmp1);
  objects.push_back(tmp2);
}

void GameMode::update(float elapsed) {
  progress += elapsed * progress_speed;
}

