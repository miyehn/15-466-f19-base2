#include "Player.hpp"
#include <iostream>

Player::Player(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_) : 
  GameObject(vertices_, position_, color_) {
}

Player::~Player() {
}

void Player::update(float elapsed) {

  // motion update
  if (current_fly_time >= fly_time_threshold) {
    acceleration = glm::vec2(0, (20.0f - position.y) * 20.0f);
  } else {
    acceleration = glm::vec2(0, -200);
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

}

void Player::draw_prep() {
  rect(position, glm::vec2(10, 10), active ? color : inactive_color);
}

void Player::jump() {
  velocity = glm::vec2(20, 100); 
}

void Player::deactivate() {
  current_inactive_time = 0.0f;
  active = false;
}
