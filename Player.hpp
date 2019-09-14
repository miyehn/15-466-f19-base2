#include "GameObject.hpp"

struct Player : GameObject {
  Player(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_);
  ~Player() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

  // player
  glm::vec2 acceleration;
  glm::vec2 velocity;
  void jump();
};
