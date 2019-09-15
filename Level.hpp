#pragma once

#include <iostream>
#include <vector>

struct Level {
  Level(const std::string &path);
  ~Level(){} 

  float time_offset = -0.0729f; 
  float visual_offset = -0.5829f;
  
  // start_measure_cnt, tempo, start_measure_cnt, tempo, ...
  std::vector<float> tempo_info = std::vector<float>(); 
  float get_time(int measure, float note);
  std::vector<float> get_measure_n_note(float time);

  float get_note_length(int measure, float note);
  void update_note_length(int measure, float note);
  float note_length; // updated

  float max_height, float_height, speed; // parsed

};
