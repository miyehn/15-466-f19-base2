#include "GameObject.hpp"

struct Obstacle : GameObject {

  Obstacle(std::vector<Vertex> &vertices_, Level *lv_, glm::vec2 position_, glm::u8vec4 color_) : 
    GameObject(vertices_, lv_, position_, color_) {}
  ~Obstacle() override;

  // inherited
  void update(float elapsed, float min_x, float max_x) override;
  void draw_prep() override;

};
