#pragma once
#include <unordered_map>
#include <utility>
#include <string>

struct sData {
  std::string name;
  std::pair<int, int> pos;
};

struct sIsland {
  std::string name;
  std::vector<std::pair<int, int>> points;
};

class CServerData
{
public:

  std::string comando(const std::vector<std::string> &tokens);


private:
  std::string msgAddUser(std::string name);
  std::string msgAddMap(int id, std::string name, std::vector<std::pair<int, int>> points);

  std::string msgGetMap();

  std::pair<int, int> getNewPosition();
  int getNameID(std::string name);

private:
	std::unordered_map<int, sData> mUsers;
  std::unordered_map<int, sIsland> mIslands;
};

