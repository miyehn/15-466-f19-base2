#include "GameMode.hpp"
#include <iostream>

void GameMode::init() {
  
  // player
  player = new Player(vertices, glm::vec2(50, 0), glm::u8vec4(200, 200, 200, 255));
  objects.push_back(player);

  // obstacles
  for (int i=0; i<5; i++) {
    Obstacle* obs = new Obstacle(vertices, glm::vec2(200 + 100*i, 0), glm::u8vec4(200, 100, 0, 255));
    obstacles.push_back(obs);
    objects.push_back(obs);
  }

}

void GameMode::update(float elapsed) {

  // update game progress
  progress += elapsed * progress_speed;
  min_x = progress - padding;
  max_x = progress + scene_size.x + padding;

  // update each game object
  for (auto object : objects) {
    if (object->position.x >= min_x && object->position.x <= max_x) object->update(elapsed);
  }

  // collisions
  for (auto obstacle : obstacles) {
    float dist = glm::distance(obstacle->position, player->position);
    // std::cout << dist << std::endl;
    if (dist < 5.0f) player->deactivate();
  }
}

bool GameMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
  if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_a) {
    player->jump();
  }
  return false;
}

