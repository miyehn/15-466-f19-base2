#include "ColorTextureProgram.hpp"
#include "GL.hpp"
#include "Mode.hpp"
#include "Sound.hpp"

struct StoryMode : Mode {
  StoryMode();
  virtual ~StoryMode();

  virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
  virtual void update(float elapsed) override;
  virtual void draw(glm::uvec2 const &drawable_size) override;
  // background music
  std::shared_ptr< Sound::PlayingSample > background_music;

  // game states
  glm::uvec2 scene_size = glm::uvec2(720, 480); // will use this size to fill entire window
  float horizon_height = 200.0f; // how far is the floor from bottom of canvas
  float progress = 0.0f; // how many pixels has the window displaced already
  float progress_speed = 20.0f; // how many pixels does the window displace per second

  //------ OpenGL-related (taken from game0 base code) ------
  //draw functions will work on vectors of vertices, defined as follows:
  struct Vertex {
    Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
      Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
    glm::vec3 Position;
    glm::u8vec4 Color;
    glm::vec2 TexCoord;
  };
  static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "TetMode::Vertex should be packed");

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
