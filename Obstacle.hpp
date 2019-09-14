#include "GameObject.hpp"

struct Obstacle : GameObject {

  Obstacle(std::vector<Vertex> &vertices_, glm::vec2 position_);
  ~Obstacle() override;

  // inherited
  void update(float elapsed) override;
  void draw_prep() override;

  glm::vec2 position;
  glm::u8vec4 color = glm::u8vec4(255, 255, 0, 255);
};
