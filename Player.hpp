#include "GameObject.hpp"

struct Player : GameObject {
  Player(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_);
  ~Player() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

  //---- player ----

  // active/inactive due to obstacles
  bool active = true;
  glm::u8vec4 inactive_color = glm::u8vec4(100, 100, 100, 255);
  float max_inactive_time = 2.0f;
  float current_inactive_time = 0.0f;

  // jump/float due to short/long press
  bool on_ground = true;
  bool key_pressed = false;
  bool key_pressed_since_jump = false;
  float fly_time_threshold = 0.5f;
  float current_fly_time = 0.0f;

  glm::vec2 acceleration = glm::vec2(0, -200);
  glm::vec2 velocity = glm::vec2(20, 0);
  void jump();
  void deactivate();
};
