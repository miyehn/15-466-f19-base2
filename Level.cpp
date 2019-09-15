#include "Level.hpp"
#include <fstream>
#include <glm/glm.hpp>

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
    if (line.compare("--")==0) {
      parse_stage++; continue;
    }

    // parsing, based on: http://www.cplusplus.com/reference/cstdio/sscanf/
    if (parse_stage == 0) { // tempo info
      float start_measure_cnt, tempo;
      assert(sscanf(line.c_str(), "%f %f", &start_measure_cnt, &tempo)==2);
      tempo_info.push_back(start_measure_cnt);
      tempo_info.push_back(tempo);
    } else if (parse_stage == 1) { // jump height
      assert(sscanf(line.c_str(), "%f %f", &max_height, &float_height)==2);
    } else {
      std::cout << line << std::endl;
    }

  }
  std::cout << "---- done ----" << std::endl;
  
  /*
   * TODO: either get_note_length or get_measure_n_note is buggy.
   * The first 60 measures are fine but after that hmm.. uncomment below to test
  std::cout << get_time(1, 0.0f) << std::endl;
  std::cout << get_time(2, 0.0f) << std::endl;
  std::cout << get_time(3, 0.0f) << std::endl;
  std::cout << get_time(26, 0.0f) << std::endl;
  std::cout << get_time(28, 0.0f) << std::endl;
  std::cout << get_time(45, 2.0f) << std::endl;
  std::cout << get_time(62, 0.0f) << std::endl;
  std::cout << get_time(165, 0.0f) << std::endl;
  std::cout << "--------" << std::endl;
  std::vector<float> res = get_measure_n_note(-0.5829);
  std::cout << res[0] << ", " << res[1] << std::endl;
  res = get_measure_n_note(1.20814);
  std::cout << res[0] << ", " << res[1] << std::endl;
  res = get_measure_n_note(2.99919);
  std::cout << res[0] << ", " << res[1] << std::endl;
  res = get_measure_n_note(44.1932);
  std::cout << res[0] << ", " << res[1] << std::endl;
  res = get_measure_n_note(48.1932);
  std::cout << res[0] << ", " << res[1] << std::endl;
  res = get_measure_n_note(79.5365);
  std::cout << res[0] << ", " << res[1] << std::endl;
  res = get_measure_n_note(109.998);
  std::cout << res[0] << ", " << res[1] << std::endl;
  res = get_measure_n_note(294.476);
  std::cout << res[0] << ", " << res[1] << std::endl;
  */
}

void Level::update_note_length(int measure, float note) {
  note_length = get_note_length(measure, note);
}

// count measure from 1, count note from 0
float Level::get_note_length(int measure, float note) {
  assert(measure>0 && note>=0 && note<=4);
  for (int i=0; i<tempo_info.size()-2; i+=2) {
    if (measure >= tempo_info[i] && measure < tempo_info[i+2])
      return 60.0f / tempo_info[i+1];
  }
  return 60.0f / tempo_info[tempo_info.size()-1];
}

// count measure from 1, count note from 0
float Level::get_time(int measure, float note) {

  assert(measure>0 && note>=0 && note<=4);
  float cumulative = time_offset;

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

// count measure from 1, count note from 0
std::vector<float> Level::get_measure_n_note(float time) {
  float cumulative = time - time_offset;
  float measure_cnt = 1.0f;
  float cumulative_section_length = 0.0f;
  for (int i=0; i<tempo_info.size()-2; i+=2) {
    measure_cnt = tempo_info[i];
    float note_length = 60.0f / tempo_info[i+1];
    float note_length_4 = 4 * note_length;
    float section_length = (tempo_info[i+2] - tempo_info[i]) * note_length_4;
    if (cumulative <= cumulative_section_length + section_length) {
      while (cumulative - cumulative_section_length >= note_length_4) {
        cumulative -= note_length_4;
        measure_cnt++;
      }
      float measure_cnt_int = glm::floor(measure_cnt);
      float resid_add_to_note = (measure_cnt - measure_cnt_int) * note_length;
      float note_cnt = resid_add_to_note + 
        (cumulative - cumulative_section_length) / note_length;
      return {measure_cnt_int, note_cnt};
    }
    cumulative_section_length += section_length;
  }
  measure_cnt = tempo_info[tempo_info.size()-2];

  float note_length = 60.0f / tempo_info[tempo_info.size()-1];
  float note_length_4 = note_length * 4;
  while (cumulative - cumulative_section_length >= note_length_4) {
    cumulative -= note_length_4;
    measure_cnt++;
  }
  float measure_cnt_int = glm::floor(measure_cnt);
  float resid_add_to_note = (measure_cnt - measure_cnt_int) * note_length;
  float note_cnt = resid_add_to_note + 
    (cumulative - cumulative_section_length) / note_length;
  return {measure_cnt_int, note_cnt};
}

