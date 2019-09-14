#include "Player.hpp"
#include <iostream>

Player::Player(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_) : 
  GameObject(vertices_, position_, color_) {
  acceleration = glm::vec2(0, -200);
  velocity = glm::vec2(20, 0);
}

Player::~Player() {
}

void Player::update(float elapsed) {
  // motion update
  velocity += acceleration * elapsed;
  position += velocity * elapsed;
  if (position.y<0) position.y = 0; // clamp to ground

  // active status
  if (!active) {
    current_inactive_time += elapsed;
    if (current_inactive_time >= max_inactive_time) active = true;
  }
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
