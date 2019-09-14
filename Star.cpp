#include "Star.hpp"
#include <glm/gtc/constants.hpp>

Star::~Star() {
}

void Star::update(float elapsed) {
  if (current_exploding_time >= max_exploding_time) active = false;
  if (exploding) {
    current_exploding_time += elapsed;
    radius += 20 * elapsed;
  }
}

void Star::draw_prep() {
  if (!active) return;
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
