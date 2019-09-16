#include "MasterMode.hpp"
#include "Sound.hpp"
#include "Level.hpp"
#include "data_path.hpp"

Load< Sound::Sample > soundtrack(LoadTagDefault, []() -> Sound::Sample * {
  return new Sound::Sample(data_path("track.wav"));
});

Level *level_from_file_left = nullptr;
Level *level_from_file_right = nullptr;

Load< Level > level_load_from_file_left(LoadTagDefault, []() -> Level * {
  level_from_file_left = new Level(data_path("level"));
  level_from_file_right = new Level(data_path("level"));
  return level_from_file_left;
});

MasterMode::MasterMode() {

  left = new GameMode(level_from_file_left);
  left->key = SDLK_a;

  right = new GameMode(level_from_file_right);
  right->up_side_down = true;
  right->key = SDLK_l;

  background_music = Sound::play(*soundtrack, 1.0f);
}

MasterMode::~MasterMode() {
  delete left;
  delete right;
}

void MasterMode::update(float elapsed) {
  left->update(elapsed);
  right->update(elapsed);
}

void MasterMode::draw(glm::uvec2 const &drawable_size) {

  //clear the color buffer:
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  left->draw(drawable_size);
  right->draw(drawable_size);
  
}

bool MasterMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
  return left->handle_event(evt, window_size) || right->handle_event(evt, window_size);
}
