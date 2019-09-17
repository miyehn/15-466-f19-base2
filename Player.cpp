#include "Player.hpp"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include "GL.hpp"

/* lmao throw back to high school calc and kinematics... 
 * found some math help from: https://courses.lumenlearning.com/boundless-physics/chapter/periodic-motion/
 */

Player::Player(std::vector<Vertex> &vertices_, Level *lv_, glm::vec2 position_, glm::u8vec4 color_) : 
    GameObject(vertices_, lv_, position_, color_) {

  four_pi_sq = glm::pow<float>(glm::pi<float>(), 2) * 4.0f;
  horizontal_speed = level->speed; 
  velocity = glm::vec2(level->speed, 0); 
  jump_period = level->note_length * 2.0f;
  fly_height = level->float_height;
  gravity = glm::vec2(0, -8.0f * level->max_height / jump_period / jump_period);
  acceleration = gravity;
  jump_thrust = glm::vec2(level->speed, -gravity.y * jump_period / 2.0f);
  fly_time_threshold = jump_period / 2.0f;
  bullet_energy_threshold = level->note_length / 2.0f;
}

Player::~Player() {
}

void Player::update(float elapsed, float min_x, float max_x) {
  GameObject::update(elapsed, min_x, max_x);

  // update constants from level change
  jump_period = level->note_length * 2.0f;
  fly_height = level->float_height;
  if (!in_doublejump) gravity = glm::vec2(0, -8.0f * level->max_height / jump_period / jump_period);
  acceleration = gravity;
  jump_thrust = glm::vec2(level->speed, -gravity.y * jump_period / 2.0f);
  fly_time_threshold = jump_period / 2.0f;

  // motion update
  if (key_pressed_since_jump && current_fly_time >= fly_time_threshold) {
    acceleration = glm::vec2(0, (fly_height - position.y) * four_pi_sq / jump_period / jump_period);
  } else {
    acceleration = gravity;
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
  if (on_ground) {
    current_fly_time = 0.0f;
    in_doublejump = false;
  }
  else current_fly_time += elapsed; // not on ground, key pressed since jump

  // shooting
  if (preparing_shoot) bullet_energy += elapsed;
}

void Player::draw_prep() {

  if (level->debug) {
    // draw measure bars for debug
    glm::u8vec4 tmp_col(80, 80, 80, 255);
    for (int i=1; i<=27; i++) {
      float x0 = level->get_time(i, 0.0f) * horizontal_speed;
      float x1 = level->get_time(i, 1.0f) * horizontal_speed;
      float x2 = level->get_time(i, 2.0f) * horizontal_speed;
      float x3 = level->get_time(i, 3.0f) * horizontal_speed;
      rect(glm::vec2(x0,50), glm::vec2(1, 49), tmp_col);
      rect(glm::vec2(x1,50), glm::vec2(0.5, 49), tmp_col);
      rect(glm::vec2(x2,50), glm::vec2(0.5, 49), tmp_col);
      rect(glm::vec2(x3,50), glm::vec2(0.5, 49), tmp_col);
    }
    if (in_doublejump) {
      rect(glm::vec2(position.x, height_since_takeoff(current_fly_time, -1.0f, true)), glm::vec2(100, 1), tmp_col);
    }
  }

  rect(position, glm::vec2(6, 6), active ? color : inactive_color);
}

void Player::jump() {
  velocity = jump_thrust;
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
      *vertices, level, position, glm::u8vec4(255,255,255,255), bullet_energy, horizontal_speed); 
  bullet_energy = 0.0f;
  return bullet;
}

void Player::cancel_shoot() {
  preparing_shoot = false;
  bullet_energy = 0.0f;
}

float Player::height_since_takeoff(float t, float release_t, bool doublejump) {
  if (doublejump) {
    float doublejump_period = level->note_length * 4.0f;
    float g = -8.0f * level->max_height*1.5f / doublejump_period / doublejump_period;
    float v0 = -g * doublejump_period / 2.0f;
    return v0*t + g * t*t / 2.0f;
  } else if (t<release_t && t >= fly_time_threshold){ // floating
    float period = (t/jump_period) * glm::two_pi<float>();
    return fly_height + (fly_height - level->max_height) * glm::cos(period);
  } else if (release_t <= fly_time_threshold) { // during take off or falling off, never floated
    float v0 = jump_thrust.y;
    float g = gravity.y;
    return v0*t + g * t*t / 2.0f;
  } else if (release_t >=0) { // falling after floating
    float release_h = height_since_takeoff(release_t, release_t + epsilon);
    float dt = t - release_t;
    float period = (t/jump_period) * glm::two_pi<float>();
    float v0 = (fly_height - level->max_height) * glm::sin(period); 
    float g = gravity.y;
    return release_h + v0*dt + g * dt*dt / 2.0f;
  } else {
    return 0.0f;
  }
}

void Player::doublejump() {
  if (in_doublejump || !on_ground) return;
  in_doublejump = true;
  float doublejump_period = level->note_length * 4.0f;
  gravity = glm::vec2(0, -8.0f * level->max_height*1.5f / doublejump_period / doublejump_period);
  velocity = glm::vec2(level->speed, -gravity.y * doublejump_period / 2.0f);
}
