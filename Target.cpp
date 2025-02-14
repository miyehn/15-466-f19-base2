#include "Target.hpp"
#include <glm/gtc/constants.hpp>

Target::~Target() {
}

void Target::update(float elapsed, float min_x, float max_x) {
  GameObject::update(elapsed, min_x, max_x);
  if (position.x < min_x) { dead = true; return; }
  if (current_exploding_time >= max_exploding_time) dead = true;
  if (exploding) {
    current_exploding_time += elapsed;
    radius += 20 * elapsed;
  }
}

void Target::draw_prep() {
  if (!exploding) {
    if (destructive) rect(position, glm::vec2(4, 4), color);
    else rect(position, glm::vec2(2, 2), color);
  } else { // exploding
    if (destructive) {
      for (float a = 0.0f; a < glm::two_pi<float>(); a+= glm::two_pi<float>()/8) {
        glm::vec2 offset(glm::cos(a), glm::sin(a));
        rect(position + radius * offset, glm::vec2(2, 2), color);
      }
    } else {
      for (float a = 0.0f; a < glm::two_pi<float>(); a+= glm::two_pi<float>()/4) {
        glm::vec2 offset(glm::cos(a), glm::sin(a));
        rect(position + radius * offset, glm::vec2(2, 2), color);
      }
    }
  }
}

void Target::explode() {
  if (destructive) exploding = true;
  else dead = true;
}
