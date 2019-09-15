#include "Star.hpp"
#include <glm/gtc/constants.hpp>

Star::~Star() {
}

void Star::update(float elapsed, float min_x, float max_x) {
  GameObject::update(elapsed, min_x, max_x);
  if (exploding) {
    current_exploding_time += elapsed;
    radius += 20 * elapsed;
  }
  if (current_exploding_time >= max_exploding_time) dead = true;
  if (position.x < min_x) dead = true;
}

void Star::draw_prep() {
  if (!exploding) rect(position, glm::vec2(4, 4), color);
  else {
    for (float a = 0.0f; a < glm::two_pi<float>(); a+= glm::two_pi<float>()/6) {
      glm::vec2 offset(glm::cos(a), glm::sin(a));
      rect(position + radius * offset, glm::vec2(2, 2), color);
    }
  }
}

void Star::explode() {
  exploding = true;
}
