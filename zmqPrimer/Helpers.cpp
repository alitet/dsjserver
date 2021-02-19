#include "Helpers.h"
#include <sstream>
#include <cmath>

std::vector<std::string> tokenizer(std::string fullString, char delim)
{
  std::vector<std::string> tokens;
  std::stringstream check1(fullString);

  std::string tmp;
  while (getline(check1, tmp, delim))
  {
    tokens.push_back(tmp);
  }
  return tokens;
}


int strToInt(std::string numStr)
{
  std::stringstream strNum(numStr);
  int nume = 0;
  strNum >> nume;
  return nume;
}

std::string intToStr(int num) 
{  
  std::stringstream ss;
  ss << num;
  return ss.str();
}

std::string intToStr(size_t num)
{
  std::stringstream ss;
  ss << num;
  return ss.str();
}

float distance(int x1, int y1, int x2, int y2)
{
  int xsq = (x2 - x1) * (x2 - x1);
  int ysq = (y2 - y1) * (y2 - y1);
  return sqrtf(static_cast<float>(xsq + ysq));
}
