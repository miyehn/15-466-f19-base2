#include "ColorTextureProgram.hpp"
#include "GL.hpp"
#include "Mode.hpp"
#include "Sound.hpp"
#include "GameObject.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"
#include "Star.hpp"
#include "Bullet.hpp"
#include "Target.hpp"

struct GameMode : Mode {
  GameMode();
  virtual ~GameMode();

  virtual bool handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) override;
  void init();
  virtual void update(float elapsed) override;
  virtual void draw(glm::uvec2 const &drawable_size) override;
  // background music
  std::shared_ptr< Sound::PlayingSample > background_music;

  //------ game states ------
  glm::vec2 scene_size = glm::vec2(720, 480); // will use this size to fill entire window
  float padding = 0.0f; // outside of this range, things don't get drawn (Q: update?)
  float horizon_height = 200.0f; // how far is the floor from bottom of canvas
  float progress = 0.0f; // how many pixels has the window displaced already
  float progress_speed = 20.0f; // how many pixels does the window displace per second
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
