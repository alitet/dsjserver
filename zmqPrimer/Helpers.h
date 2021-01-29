#pragma once
#include <string>
#include <vector>

std::vector<std::string> tokenizer(std::string fullString, char delim);
int strToInt(std::string numStr);
std::string intToStr(int num);
float distance(int x1, int y1, int x2, int y2);