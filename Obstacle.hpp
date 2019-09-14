#include "GameObject.hpp"

struct Obstacle : GameObject {

  Obstacle(std::vector<Vertex> &vertices_, glm::vec2 position_, glm::u8vec4 color_) : 
    GameObject(vertices_, position_, color_) {}
  ~Obstacle() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

};
