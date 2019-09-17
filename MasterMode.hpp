#include "GameMode.hpp"

struct MasterMode : Mode {
  MasterMode();
  virtual ~MasterMode();

  virtual bool handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) override;
  virtual void update(float elapsed) override;
  virtual void draw(glm::uvec2 const &drawable_size) override;
  // background music
  std::shared_ptr< Sound::PlayingSample > background_music;

  GameMode* left = nullptr;
  GameMode* right = nullptr;

  glm::u8vec4 left_player_c = glm::u8vec4(255, 124, 0, 255);
  glm::u8vec4 left_target_c = glm::u8vec4(179, 58, 30, 255);
  glm::u8vec4 left_star_c = glm::u8vec4(255, 210, 100, 255);

  glm::u8vec4 right_player_c = glm::u8vec4(63, 57, 81, 255);
  glm::u8vec4 right_target_c = glm::u8vec4(67, 52, 187, 255);
  glm::u8vec4 right_star_c = glm::u8vec4(104, 135, 187, 255);

  glm::u8vec4 obstacle_c = glm::u8vec4(98, 98, 98, 255);

};
