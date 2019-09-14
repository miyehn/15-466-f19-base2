#include "Player.hpp"

Player::Player(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_) : 
  GameObject(vertices_, position_, color_) {
  velocity = glm::vec2(20, 0);
}

Player::~Player() {
}

void Player::update(float elapsed) {
  position += velocity * elapsed;
}

void Player::draw_prep() {
  rect(position, glm::vec2(10, 10), color);
}
