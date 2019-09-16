#include "GameObject.hpp"

struct Star : GameObject {

  Star(std::vector<Vertex> &vertices_, Level *lv_, glm::vec2 position_, glm::u8vec4 color_) : 
    GameObject(vertices_, lv_, position_, color_) { max_exploding_time = level->note_length; }
  ~Star() override;

  // inherited
  void update(float elapsed, float min_x, float max_x) override;
  void draw_prep() override;

  // star-specific
  float radius = 4.0f;

  bool exploding = false;
  float max_exploding_time = 0.0f;
  float current_exploding_time = 0.0f;
  void explode();

};
