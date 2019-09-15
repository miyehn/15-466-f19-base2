#include "Player.hpp"
#include <iostream>
#include <glm/gtc/constants.hpp>

Player::~Player() {
}

void Player::update(float elapsed, float min_x, float max_x) {
  GameObject::update(elapsed, min_x, max_x);

  // motion update
  if (current_fly_time >= fly_time_threshold) {
    acceleration = glm::vec2(0, (25.5f - position.y) * glm::pow<float>(glm::pi<float>(), 2) * 4.0f);
  } else {
    acceleration = glm::vec2(0, -400);
  }
  velocity += acceleration * elapsed;
  position += velocity * elapsed;
  if (position.y<0) position.y = 0; // clamp to ground

  // active status
  if (!active) {
    current_inactive_time += elapsed;
    if (current_inactive_time >= max_inactive_time) active = true;
  }

  // fly time update
  on_ground = position.y < epsilon;
  if (on_ground || !key_pressed_since_jump) current_fly_time = 0.0f;
  else current_fly_time += elapsed; // not on ground, key pressed since jump

  // shooting
  if (preparing_shoot) bullet_energy += elapsed;
}

void Player::draw_prep() {
  glm::u8vec4 tmp_col(80, 80, 80, 255);
  for (int i=1; i<=165; i++) {
    float x0 = level->get_time(i, 0.0f) * horizontal_speed;
    float x1 = level->get_time(i, 1.0f) * horizontal_speed;
    float x2 = level->get_time(i, 2.0f) * horizontal_speed;
    float x3 = level->get_time(i, 3.0f) * horizontal_speed;
    rect(glm::vec2(x0,0), glm::vec2(1, 100), tmp_col);
    rect(glm::vec2(x1,0), glm::vec2(0.5, 100), tmp_col);
    rect(glm::vec2(x2,0), glm::vec2(0.5, 100), tmp_col);
    rect(glm::vec2(x3,0), glm::vec2(0.5, 100), tmp_col);
  }
  rect(position, glm::vec2(10, 10), active ? color : inactive_color);
}

void Player::jump() {
  velocity = glm::vec2(horizontal_speed, 200); // TODO
}

void Player::deactivate() {
  current_inactive_time = 0.0f;
  active = false;
}

void Player::prepare_shoot() {
  preparing_shoot = true;
}

Bullet* Player::shoot() {
  preparing_shoot = false;
  Bullet* bullet = new Bullet(
      *vertices, position, glm::u8vec4(255,255,255,255), bullet_energy, horizontal_speed); 
  bullet_energy = 0.0f;
  return bullet;
}

void Player::cancel_shoot() {
  preparing_shoot = false;
  bullet_energy = 0.0f;
}
