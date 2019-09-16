#include "GameMode.hpp"
#include <iostream>

void GameMode::init() {
  assert(level != nullptr);

  for (auto line : level->level_info) {
    std::cout << line << std::endl;
  }
  
  // player
  player = new Player(vertices, level, glm::vec2(50, 0), glm::u8vec4(200, 200, 200, 255));
  objects.push_back(player);
  player->level = this->level;

  // obstacles
  for (int i=0; i<5; i++) {
    Obstacle* obs = new Obstacle(vertices, level, glm::vec2(200 + 100*i, 0), glm::u8vec4(200, 100, 0, 255));
    obstacles.push_back(obs);
    objects.push_back(obs);
  }
  
  // stars
  for (int i=0; i<4; i++) {
    Star* star = new Star(vertices, level, glm::vec2(300 + 150*i, 20), glm::u8vec4(255, 220, 0, 255));
    stars.push_back(star);
    objects.push_back(star);
  }
  
  for (int i=0; i<10; i++) {
    Star* star = new Star(vertices, level, glm::vec2(100 + 40*i, 50), glm::u8vec4(255, 0, 0, 255));
    stars.push_back(star);
    objects.push_back(star);
  }

  // targets
  for (int i=0; i<5; i++) {
    if (i==4 || i==0) {
      Target* target = new Target(
          vertices, level, glm::vec2(500 + 80*i, 100), glm::u8vec4(200, 80, 255, 255), 0.2f, true);
      targets.push_back(target);
      objects.push_back(target);
    } else {
      Target* target = new Target(vertices, level, glm::vec2(500 + 80*i, 100), glm::u8vec4(200, 80, 255, 255), 0.0f);
      targets.push_back(target);
      objects.push_back(target);
    }
  }

  // testing - looks good now.
  for (int i=0; i<40; i++) {
    Target* t = new Target(vertices, level, glm::vec2(
          200 + i*level->note_length/4.0f*level->speed, 
          player->height_since_takeoff(i*level->note_length/4.0f, level->note_length * 7)),
        glm::u8vec4(100,255,100,255), 0.0f);
    targets.push_back(t);
    objects.push_back(t);
  }
}

void GameMode::update(float elapsed) {

  // update game progress
  time_since_start += elapsed;
  progress += elapsed * level->speed;

  min_x = progress - padding;
  max_x = progress + scene_size.x + padding;

  level->update(elapsed);

  // update each game object
  for (auto object : objects) {
    object->update(elapsed, min_x, max_x);
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

  // bullet-target collision
  for (auto bullet : bullets) {
    for (auto target : targets) {
      float dist = glm::distance(bullet->position, target->position);
      if (bullet->energy>=target->energy && dist < 5.0f) {
        target->explode();
        if (target->destructive) bullet->dead = true;
      }
    }
  }

  // clean out dead elements
  for (int i=0; i<objects.size(); i++) {
    if (objects[i]->dead) { 
      objects.erase(objects.begin() + i);
      i--;
    }
  }

  // std::cout << objects.size() << std::endl;
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
    } else if (player->active) {
      player->prepare_shoot();
    }
    return true;

  } else if (evt.type == SDL_KEYUP && evt.key.keysym.sym == SDLK_a) {
    // unlock this key
    player->key_pressed = false;

    player->key_pressed_since_jump = false;

    // shooting
    if (player->preparing_shoot) {
      if (player->on_ground) {
        player->cancel_shoot();
      } else {
        Bullet* bullet = player->shoot();
        bullets.push_back(bullet);
        objects.push_back(bullet);
      }
    }
    return true;
  }
  return false;
}

