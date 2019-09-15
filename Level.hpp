#include <iostream>
#include <vector>

struct Level {
  Level(const std::string &path);
  ~Level(){} 
  
  // start_measure_cnt, tempo, start_measure_cnt, tempo, ...
  std::vector<float> tempo_info = std::vector<float>(); 
  float get_time(int measure, float note);

};
