#include "Player.hpp"
#include <iostream>
#include <glm/gtc/constants.hpp>

Player::Player(std::vector<Vertex> &vertices_, Level *lv_, glm::vec2 position_, glm::u8vec4 color_) : 
    GameObject(vertices_, lv_, position_, color_) { 

  // lmao throw back to high school calc and kinematics... 
  // some math help from: https://courses.lumenlearning.com/boundless-physics/chapter/periodic-motion/
  four_pi_sq = glm::pow<float>(glm::pi<float>(), 2) * 4.0f;
  horizontal_speed = level->speed; 
  velocity = glm::vec2(level->speed, 0); 
  jump_period = level->note_length * 2.0f;
  fly_height = level->float_height;
  gravity = glm::vec2(0, -8.0f * level->max_height / jump_period / jump_period);
  acceleration = gravity;
  jump_thrust = glm::vec2(level->speed, -gravity.y * jump_period / 2.0f);
  fly_time_threshold = jump_period / 2.0f;
}

Player::~Player() {
}

void Player::update(float elapsed, float min_x, float max_x) {
  GameObject::update(elapsed, min_x, max_x);

  // update constants from level change
  jump_period = level->note_length * 2.0f;
  fly_height = level->float_height;
  gravity = glm::vec2(0, -8.0f * level->max_height / jump_period / jump_period);
  acceleration = gravity;
  jump_thrust = glm::vec2(level->speed, -gravity.y * jump_period / 2.0f);
  fly_time_threshold = jump_period / 2.0f;

  // motion update
  if (current_fly_time >= fly_time_threshold) {
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
  if (on_ground || !key_pressed_since_jump) current_fly_time = 0.0f;
  else current_fly_time += elapsed; // not on ground, key pressed since jump

  // shooting
  if (preparing_shoot) bullet_energy += elapsed;
}

void Player::draw_prep() {

  if (level->debug) {
    // draw measure bars for debug
    glm::u8vec4 tmp_col(80, 80, 80, 255);
    glm::u8vec4 magenta(255,0,255,255);
    for (int i=1; i<=165; i++) {
      float x0 = level->get_time(i, 0.0f) * horizontal_speed;
      float x1 = level->get_time(i, 1.0f) * horizontal_speed;
      float x2 = level->get_time(i, 2.0f) * horizontal_speed;
      float x3 = level->get_time(i, 3.0f) * horizontal_speed;
      rect(glm::vec2(x0,0), glm::vec2(1, 100), tmp_col);
      rect(glm::vec2(x1,0), glm::vec2(0.5, 100), tmp_col);
      rect(glm::vec2(x2,0), glm::vec2(0.5, 100), tmp_col);
      rect(glm::vec2(x3,0), glm::vec2(0.5, 100), tmp_col);
    }
    if (!on_ground) {
      rect(glm::vec2(position.x, height()),
          glm::vec2(20,0.5), magenta);
    }
  }

  rect(position, glm::vec2(10, 10), active ? color : inactive_color);
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

float Player::height() {
  if (on_ground) return 0.0f;
  else if (current_fly_time <= fly_time_threshold) {
    float t = current_fly_time;
    float v0 = jump_thrust.y;
    float g = gravity.y;
    return v0*t + g * t*t / 2.0f;
  } else {
    float period = (current_fly_time/jump_period) * glm::two_pi<float>();
    return fly_height + (fly_height - level->max_height) * glm::cos(period);
  }
}
