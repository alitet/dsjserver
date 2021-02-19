#pragma once
#include <unordered_map>
#include <utility>
#include <string>

//struct sData {
//  std::string name;
//  //std::pair<int, int> pos;
//};

struct sIsland {
  std::string name;
  std::pair<int, int> pos;
  std::vector<std::pair<int, int>> points;
};

class CServerData
{
public:

  std::string comando(const std::vector<std::string> &tokens);

  void addFakeUsers();
  void addFakeIslands();

private:
  std::string msgAddUser(std::string name);
  std::string msgAddMap(int id, std::string name, std::vector<std::pair<int, int>> points);
  std::string msgNumIds();
  std::string msgGetIslandData(int id);
  std::string msgGetIslandPoints(int id);

  //std::string msgGetMap();

  std::pair<int, int> getNewPosition();
  int getNameID(std::string name);

private:
	std::unordered_map<int, std::string> mUsers;
  std::unordered_map<int, sIsland> mIslands;
};

void getPointsInVector(const std::vector<std::string>& tokens, std::vector<std::pair<int, int>>& puntos);
