#pragma once

#include <iostream>
#include <vector>

struct Level {
  Level(const std::string &path);
  ~Level(){} 

  float timer = 0.0f;
  int measure = 1;
  float note = 0.0f;

  float time_offset = -0.0729f; 
  float visual_offset = -0.5829f;
  
  // start_measure_cnt, tempo, start_measure_cnt, tempo, ...
  std::vector<float> tempo_info = std::vector<float>(); 
  std::vector<std::string> level_info = std::vector<std::string>();
  float get_time(int measure, float note);
  std::vector<float> get_measure_n_note(float time);

  float get_note_length(int measure, float note);
  void update_note_length(int measure, float note);
  float note_length; // updated

  float max_height, float_height, speed; // parsed
  bool debug, autoplay; // parsed

  void update(float elapsed);

};
