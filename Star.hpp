#include "GameObject.hpp"

struct Star : GameObject {

  Star(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_) : 
    GameObject(vertices_, position_, color_) {}
  ~Star() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

  // star-specific
  bool active = true;
  float radius = 4.0f;

  bool exploding = false;
  float max_exploding_time = 1.0f;
  float current_exploding_time = 0.0f;
  void explode();

};
