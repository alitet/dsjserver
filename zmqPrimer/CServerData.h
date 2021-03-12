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

struct sWall {
  int type;
  int pos[3];
};

class CServerData
{
public:

  std::string comando(const std::vector<std::string> &tokens);

  void addFakeUsers();
  void addFakeIslands();
  void addFakeTrees();

private:
  std::string msgAddUser(std::string name);
  std::string msgAddMap(int id, std::string name, std::vector<std::pair<int, int>> points);
  std::string msgNumIds();
  std::string msgGetIslandData(int id);
  std::string msgGetIslandPoints(int id);
  std::string msgGetUserName(int id);
  std::string msgGetTrees(int id);

  std::string msgAddTrees(int id, std::vector<std::pair<int, int>> trees);
  std::string msgAddWalls(int id, std::vector<sWall> walls);

  //std::string msgGetMap();

  std::pair<int, int> getNewPosition();
  int getNameID(std::string name);

private:
	std::unordered_map<int, std::string> mUsers;
  std::unordered_map<int, sIsland> mIslands;
  std::unordered_map<int, std::vector<std::pair<int, int>>> mTrees;
  std::unordered_map<int, std::vector<sWall>> mWalls;
};

//void getPointsInVector(const std::vector<std::string>& tokens, std::vector<std::pair<int, int>>& puntos);
