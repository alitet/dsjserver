#pragma once
#include <string>
#include <vector>

std::vector<std::string> tokenizer(std::string fullString, char delim);
int strToInt(std::string numStr);
std::string intToStr(int num);
std::string intToStr(size_t num);
float distance(int x1, int y1, int x2, int y2);
float distance(int x1, int y1, int z1, int x2, int y2, int z2);