#include "GameMode.hpp"
#include <iostream>

void GameMode::init() {
  assert(level != nullptr);

  // player
  player = new Player(vertices, level, glm::vec2(0, 0), glm::u8vec4(200, 200, 200, 255));
  objects.push_back(player);
  player->level = this->level;

  // things to keep track of when parsing
  // DO NOT SCAN INTO THESE
  int measure_cnt = 0; 
  float note_cnt = 0.0f;
  float release_n = 0.0f;
  float shooting_pos_x, shooting_pos_h, max_bullet_energy = 0.0f;
  bool in_jump = false;
  bool in_shoot = false;
  // ok to scan into
  int m = 0; float n = 0.0f;
  float r_n = 0.0f; // release offset note parsed from same line as jump
  float offset_n = 0.0f; // parsed from inside jump or shoot, note offset since last abs time
  float shoot_n = 0.0f; // parsed from inside shoot, shooting time as note offset since last abs time
  float energy_n = 0.0f; // parsed with target (whether destructive or not)

  for (auto line : level->level_info) {
    const char* l = line.c_str();
    std::cout << "----parsing: " << line << std::endl;

    // parse level info from input

    //---- begin & end jump ----
    if (sscanf(l, "%d %f jump +%f", &m, &n, &r_n)==3) { // absolute timing
      in_jump = true;
      measure_cnt = m; note_cnt = n; release_n = r_n;
      if (line.substr(line.length()-9, 9)==" obstacle") {
        float x = level->speed * (level->get_time(measure_cnt, note_cnt) + player->jump_period/2.0f);
        Obstacle* obs = new Obstacle(vertices, level, glm::vec2(x, 0), glm::u8vec4(200, 100, 0, 255));
        obstacles.push_back(obs);
        objects.push_back(obs);
      }

    } else if (line == "end jump") {
      in_jump = false;

    //---- star ----
    } else if (sscanf(l, "  +%f", &offset_n)==1 && 
        line.substr(line.length()-5,5)==" star") { // relative timing
      assert(in_jump);
      m = measure_cnt; n = note_cnt + offset_n;
      while (n >= 4) { m++; n-=4; }
      float x = level->speed * level->get_time(m, n);
      float h = player->height_since_takeoff(offset_n * level->note_length, release_n * level->note_length);
      Star* star = new Star(vertices, level, glm::vec2(x, h), glm::u8vec4(255, 220, 0, 255));
      stars.push_back(star);
      objects.push_back(star);

    //---- begin & end shooting ----
    } else if (sscanf(l, "  +%f prepare +%f", &offset_n, &shoot_n)==2 && 
        line.substr(line.length()-6,6)==" shoot") {
      assert(in_jump && !in_shoot);
      in_shoot = true;
      m = measure_cnt; n = note_cnt + shoot_n;
      while (n >= 4) { m++; n-=4; }
      shooting_pos_x = level->speed * level->get_time(m, n);
      shooting_pos_h = player->height_since_takeoff(shoot_n * level->note_length, release_n * level->note_length);
      max_bullet_energy = (shoot_n-offset_n) * level->note_length;

    } else if (line=="  end shoot") {
      in_shoot = false;

    //---- shooting ----
    } else if (sscanf(l, "    +%f >%f", &offset_n, &energy_n)==2) {
      assert(in_jump && in_shoot);
      float e = energy_n * level->note_length;
      if (e > max_bullet_energy) 
        std::cout << "WARNING: target requires more energy than max bullet energy" << std::endl;
      float d = player->horizontal_speed * offset_n*level->note_length;
      float bx = shooting_pos_x + d*2;
      float bh = shooting_pos_h + d;
      if (line.substr(line.length()-7,7)==" target") {
        Target* target = new Target(vertices, level, glm::vec2(bx, bh), glm::u8vec4(200, 80, 255, 255), e);
        targets.push_back(target);
        objects.push_back(target);
      } else if (line.substr(line.length()-12,12)==" destructive") {
        Target* destr = new Target(vertices, level, glm::vec2(bx, bh), glm::u8vec4(200, 80, 255, 255), e, true);
        targets.push_back(destr);
        objects.push_back(destr);
      }
    } else {
      std::cout << "parse failed!" << std::endl;
    }

  }
  assert(!in_jump);
  assert(!in_shoot);
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

