#include "GameObject.hpp"
#include "Bullet.hpp"
#include "Level.hpp"

struct Player : GameObject {
  Player(
      std::vector<Vertex> &vertices_, 
      Level *lv_,
      glm::vec2 position_, 
      glm::u8vec4 color_, 
      float speed) : 
    GameObject(vertices_, lv_, position_, color_) { horizontal_speed = speed; velocity = glm::vec2(speed, 0); }
  ~Player() override;

  // inherited
  void update(float elapsed, float min_x, float max_x) override;
  void draw_prep() override;

  //---- player ----

  Level *level = nullptr;

  // control status
  bool key_pressed = false;
  bool key_pressed_since_jump = false;

  // motion status
  float horizontal_speed = 0.0f;
  glm::vec2 acceleration = glm::vec2(0, -200); // TODO
  glm::vec2 velocity = glm::vec2(0, 0);

  // active/inactive due to obstacles
  bool active = true;
  glm::u8vec4 inactive_color = glm::u8vec4(100, 100, 100, 255);
  float max_inactive_time = 2.0f;
  float current_inactive_time = 0.0f;
  void deactivate();

  // jump/float due to short/long press
  bool on_ground = true;
  float fly_time_threshold = 0.5f; // TODO: should be half of t0
  float current_fly_time = 0.0f;
  void jump();

  // shooting-related
  bool preparing_shoot = false;
  float bullet_energy = 0.0f;
  void prepare_shoot();
  Bullet* shoot();
  void cancel_shoot();
};
