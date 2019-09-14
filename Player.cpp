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
  velocity += acceleration * elapsed;
  position += velocity * elapsed;
  if (position.y<0) position.y = 0; // clamp to ground

}

void Player::draw_prep() {
  rect(position, glm::vec2(10, 10), color);
}

void Player::jump() {
  velocity = glm::vec2(20, 100); 
}
