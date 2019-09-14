#include "GameMode.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"

void GameMode::init() {
  GameObject* tmp1 = new Obstacle(vertices, glm::vec2(100, 0), glm::u8vec4(255, 100, 0, 255));
  GameObject* tmp2 = new Obstacle(vertices, glm::vec2(300, 0), glm::u8vec4(255, 200, 0, 255));
  GameObject* player = new Player(vertices, glm::vec2(50, 0), glm::u8vec4(100, 100, 100, 255));
  objects.push_back(tmp1);
  objects.push_back(tmp2);
  objects.push_back(player);
}

void GameMode::update(float elapsed) {
  progress += elapsed * progress_speed;
  for (auto object : objects) {
    object->update(elapsed);
  }
}

