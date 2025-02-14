#include "GameObject.hpp"

struct Target : GameObject {

  Target(std::vector<Vertex> &vertices_, 
      Level *lv_,
      glm::vec2 position_, 
      glm::u8vec4 color_, 
      float energy_ = 0.0f,
      bool destructive_ = false) : 
    GameObject(vertices_, lv_, position_, color_) { 
      energy = energy_; 
      destructive = destructive_;
      max_exploding_time = destructive ? level->note_length*2 : level->note_length/2.0f;
    }
  ~Target() override;

  // inherited
  void update(float elapsed, float min_x, float max_x) override;
  void draw_prep() override;

  // target-specific
  float energy = 0.0f;
  bool destructive = false;

  float radius = 4.0f;

  bool exploding = false;
  float max_exploding_time = 0.0f;
  float current_exploding_time = 0.0f;
  void explode();
};
