#include "MasterMode.hpp"
#include "Sound.hpp"
#include "Level.hpp"
#include "data_path.hpp"

Load< Sound::Sample > soundtrack(LoadTagDefault, []() -> Sound::Sample * {
  return new Sound::Sample(data_path("track_47_16bit_signed.wav"));
});

Level *level_from_file_left = nullptr;
Level *level_from_file_right = nullptr;

Load< Level > level_load_from_file_left(LoadTagDefault, []() -> Level * {
  level_from_file_left = new Level(data_path("level-left"));
  level_from_file_right = new Level(data_path("level-right"));
  return level_from_file_left;
});

MasterMode::MasterMode() {

  left = new GameMode(level_from_file_left, left_player_c, left_target_c, left_star_c, obstacle_c);
  left->key = SDLK_a;

  right = new GameMode(level_from_file_right, right_player_c, right_target_c, right_star_c, obstacle_c);
  right->up_side_down = true;
  right->key = SDLK_l;

  background_music = Sound::play(*soundtrack, 1.0f);
}

MasterMode::~MasterMode() {
  delete left;
  delete right;
}

void MasterMode::update(float elapsed) {
  right->update(elapsed);
  left->update(elapsed);
}

void MasterMode::draw(glm::uvec2 const &drawable_size) {

  //clear the color buffer:
  glClearColor(0.149f, 0.149f, 0.157f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  right->draw(drawable_size);
  left->draw(drawable_size);
  
}

bool MasterMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

  if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_SPACE) { // TODO: change trigger to synced timing? how?
    left->player->doublejump();
    right->player->doublejump();
    return true;
  }
  return left->handle_event(evt, window_size) || right->handle_event(evt, window_size);
}
