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
  
  // stars
  for (int i=0; i<4; i++) {
    Star* star = new Star(vertices, glm::vec2(300 + 150*i, 20), glm::u8vec4(255, 220, 0, 255));
    stars.push_back(star);
    objects.push_back(star);
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

  // player-obstacle collision
  for (auto obstacle : obstacles) {
    float dist = glm::distance(obstacle->position, player->position);
    if (dist < 5.0f) player->deactivate();
  }

  // player-star collision
  for (auto star : stars) {
    float dist = glm::distance(star->position, player->position);
    if (player->active && dist < 5.0f) star->explode();
  }
}

bool GameMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
  if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_a) {

    // to prevent a...aaaaaaaaaaaaa
    if (player->key_pressed) return true;
    // from now on, lock this key (can no longer be pressed) until release
    player->key_pressed = true;

    // actual handling
    if (player->on_ground) {
      player->jump();
      player->key_pressed_since_jump = true;
    }
    return true;

  } else if (evt.type == SDL_KEYUP && evt.key.keysym.sym == SDLK_a) {
    // unlock this key
    player->key_pressed = false;
    player->key_pressed_since_jump = false;
    return true;
  }
  return false;
}

