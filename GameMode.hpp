#include "ColorTextureProgram.hpp"
#include "GL.hpp"
#include "Mode.hpp"
#include "Sound.hpp"
#include "Level.hpp"

#include "GameObject.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"
#include "Star.hpp"
#include "Bullet.hpp"
#include "Target.hpp"

struct GameMode : Mode {
  GameMode(Level* lv_, 
      glm::u8vec4 player_c,
      glm::u8vec4 target_c,
      glm::u8vec4 star_c,
      glm::u8vec4 obstacle_c);
  virtual ~GameMode();

  virtual bool handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) override;
  void init(Level* lv_, 
      glm::u8vec4 player_c,
      glm::u8vec4 target_c,
      glm::u8vec4 star_c,
      glm::u8vec4 obstacle_c);
  virtual void update(float elapsed) override;
  virtual void draw(glm::uvec2 const &drawable_size) override;
  Level *level = nullptr;

  //------ game states ------
  glm::vec2 scene_size = glm::vec2(720, 480); // will use this size to fill entire window
  bool up_side_down = false;
  SDL_Keycode key = SDLK_SPACE;
  float time_since_start = 0.0f;

  float padding = 20.0f; // outside of this range, things don't get drawn (Q: update?)
  float progress = -100.0f; // how many pixels has the window displaced already
  float min_x, max_x;

  // generics
  std::vector<Vertex> vertices = std::vector<Vertex>(); // gets cleared by the end of draw, maybe?
  std::vector<GameObject*> objects = std::vector<GameObject*>();

  // specifics
  Player* player;
  std::vector<Obstacle*> obstacles;
  std::vector<Star*> stars;
  std::vector<Bullet*> bullets;
  std::vector<Target*> targets;

  //------ OpenGL-related (taken from game0 base code) ------
  
  //Shader program that draws transformed, vertices tinted with vertex colors:
  ColorTextureProgram color_texture_program;

  //Buffer used to hold vertex data during drawing:
  GLuint vertex_buffer = 0;

  //Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
  GLuint vertex_buffer_for_color_texture_program = 0;

  //Solid white texture:
  GLuint white_tex = 0;

  //matrix that maps from clip coordinates to court-space coordinates:
  glm::mat3x2 clip_to_court = glm::mat3x2(1.0f);
  // computed in draw() as the inverse of OBJECT_TO_CLIP
  // (stored here so that the mouse handling code can use it to position the paddle)
};
