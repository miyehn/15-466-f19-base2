#include "GameObject.hpp"
#include "Bullet.hpp"
#include "Level.hpp"

struct Player : GameObject {
  Player(
      std::vector<Vertex> &vertices_, 
      Level *lv_,
      glm::vec2 position_, 
      glm::u8vec4 color_); 
  ~Player() override;

  // inherited
  void update(float elapsed, float min_x, float max_x) override;
  void draw_prep() override;

  //---- player ----

  // control status
  bool key_pressed = false;
  bool key_pressed_since_jump = false;

  // motion status
  float jump_period;
  float horizontal_speed;
  glm::vec2 acceleration; 
  glm::vec2 velocity = glm::vec2(0, 0);

  // active/inactive due to obstacles
  bool active = true;
  glm::u8vec4 inactive_color = glm::u8vec4(100, 100, 100, 255);
  float max_inactive_time = 2.0f;
  float current_inactive_time = 0.0f;
  void deactivate();

  // jump/float due to short/long press
  bool on_ground = true;
  bool in_doublejump = false;
  glm::vec2 jump_thrust;
  glm::vec2 gravity;

  float four_pi_sq;
  float fly_time_threshold; 
  float current_fly_time = 0.0f;
  float fly_height;
  void jump();
  void doublejump(); // invoked from MasterMode only
  float height_since_takeoff(float t, float release_t = -1.0f);

  // shooting-related
  bool preparing_shoot = false;
  float bullet_energy = 0.0f;
  void prepare_shoot();
  Bullet* shoot();
  void cancel_shoot();
};
