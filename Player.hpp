#include "GameObject.hpp"

struct Player : GameObject {
  Player(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_);
  ~Player() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

  // player
  bool active = true;
  glm::u8vec4 inactive_color = glm::u8vec4(100, 100, 100, 255);
  float max_inactive_time = 2.0f;
  float current_inactive_time = 0.0f;

  glm::vec2 acceleration;
  glm::vec2 velocity;
  void jump();
  void deactivate();
};
