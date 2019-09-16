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

};
