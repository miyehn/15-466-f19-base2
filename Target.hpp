#include "GameObject.hpp"

struct Target : GameObject {

  Target(std::vector<Vertex> &vertices_, 
      glm::vec2 position_, 
      glm::u8vec4 color_, 
      float energy_ = 0.0f,
      bool destructive_ = false) : 
    GameObject(vertices_, position_, color_) { energy = energy_; destructive = destructive_;}
  ~Target() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

  // target-specific
  float energy = 0.0f;
  bool destructive = false;

  bool active = true;
  float radius = 4.0f;

  bool exploding = false;
  float max_exploding_time = 1.0f;
  float current_exploding_time = 0.0f;
  void explode();
};
