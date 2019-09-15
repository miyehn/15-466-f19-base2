#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Level.hpp"

//draw functions will work on vectors of vertices, defined as follows:
struct Vertex {
  Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
    Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
  glm::vec3 Position;
  glm::u8vec4 Color;
  glm::vec2 TexCoord;
};
static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "GameObject: Vertex should be packed");

struct GameObject {
  GameObject(std::vector<Vertex> &vertices_, Level *level_, glm::vec2 position_, glm::u8vec4 color_) { 
    vertices = &vertices_; 
    level = level_;
    position = position_;
    color = color_;
  }
  virtual ~GameObject(){}

  virtual void update(float elapsed, float min_x, float max_x) {
    this->min_x = min_x;
    this->max_x = max_x;
  }

  // implement by calling rect(...)
  virtual void draw_prep() = 0;

  void rect(glm::vec2 const &center, 
      glm::vec2 const &radius,
      glm::u8vec4 const &color) {
    if (center.x+radius.x < min_x || center.x-radius.x > max_x) return;

    vertices->emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    vertices->emplace_back(glm::vec3(center.x+radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    vertices->emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));

    vertices->emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    vertices->emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    vertices->emplace_back(glm::vec3(center.x-radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
  }

  // properties
  bool dead = false;
  float min_x, max_x;

  glm::vec2 position;
  glm::u8vec4 color;

  std::vector<Vertex> *vertices = nullptr; // shared by every object of GameMode
  Level *level = nullptr; // has all the tempo info, etc.


  float epsilon = 0.001f;
};
