#include "Level.hpp"
#include <fstream>

Level::Level(const std::string &path) {

  std::ifstream infile(path);
  std::string line;
  std::cout << "---- loading level from file... ----" << std::endl;
  // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
  int lines_count = 0;
  int parse_stage = 0; // 0: tempo; 1: ...
  while (std::getline(infile, line)) {
    lines_count++;

    // ignore blank lines
    if (line.length() == 0) continue;

    // igore comments
    if (line[0]=='#') continue;

    // change of parse_stage
    if (line[0]=='!') {
      parse_stage++; continue;
    }

    // parsing, based on: http://www.cplusplus.com/reference/cstdio/sscanf/
    if (parse_stage == 0) { // tempo info
      float start_measure_cnt, tempo;
      assert(sscanf(line.c_str(), "%f %f", &start_measure_cnt, &tempo)==2);
      tempo_info.push_back(start_measure_cnt);
      tempo_info.push_back(tempo);
    } else {
      std::cout << line << std::endl;
    }

  }
  std::cout << "---- done ----" << std::endl;
  /*
  std::cout << get_time(1, 0.0f) << std::endl;
  std::cout << get_time(2, 0.0f) << std::endl;
  std::cout << get_time(3, 0.0f) << std::endl;
  std::cout << get_time(26, 0.0f) << std::endl;
  std::cout << get_time(28, 0.0f) << std::endl;
  std::cout << get_time(45, 2.0f) << std::endl;
  std::cout << get_time(62, 0.0f) << std::endl;
  std::cout << get_time(165, 0.0f) << std::endl;
  */
}

// count measure from 1, count note from 0
float Level::get_time(int measure, float note) {

  assert(measure>0 && note>=0 && note<=4);
  float cumulative = -0.5829f;

  for (int i=0; i<tempo_info.size()-2; i+=2) {
    float note_length = 60.0f / tempo_info[i+1];

    if (measure >= tempo_info[i] && measure < tempo_info[i+2]) {
      cumulative += ((measure-tempo_info[i]) * 4 + note) * note_length;
      return cumulative;
    } else {
      cumulative += (tempo_info[i+2]-tempo_info[i]) * 4 * note_length;
    }
  }
  float note_length = 60.0f / tempo_info[tempo_info.size()-1];
  cumulative += ((measure-tempo_info[tempo_info.size()-2]) * 4 + note) * note_length;
  return cumulative;
}
