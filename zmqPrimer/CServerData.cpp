#include "CServerData.h"
#include "Helpers.h"

std::string CServerData::comando(const std::vector<std::string> &tokens)
{
  std::string retStr;
  int cmmd = strToInt(tokens[0]);

  switch (cmmd) {
  case 1:
    retStr = msgAddUser(tokens[1]);
    break;
  case 4: {
    int id = strToInt(tokens[1]);
    std::string name = tokens[2];

    std::vector<std::pair<int, int>> puntos;
    for (int i = 0; i < strToInt(tokens[4]); i+=2) {
      int lx = strToInt(tokens[5 + i]);
      int ly = strToInt(tokens[5 + i + 1]);
      puntos.push_back(std::make_pair(lx, ly));
    }
    retStr = msgAddMap(id, name, puntos);

  } break;
  default: break;
  }

	return retStr;
}

std::pair<int, int> CServerData::getNewPosition()
{
  bool haspos = false;
  std::pair<int, int> retPos;

  do {
    int xx = rand() % 949;
    int yy = rand() % 949;
    bool found = true;
    for (const auto& val : mUsers) {
      int xval = val.second.pos.first;
      int yval = val.second.pos.second;
      if (distance(xx, yy, xval, yval) < 71.f) {
        found = false;
        break;
      }
    }
    if (found) {
      retPos = std::make_pair(xx, yy);
      haspos = true;
    }
  } while (!haspos);
  return retPos;
}

int CServerData::getNameID(std::string name)
{
  for (const auto& val : mUsers) {
    if (val.second.name == name) {
      return val.first;
    }
  }
  return 0;
}

std::string CServerData::msgAddUser(std::string name)
{
  std::string retStr("02/");
  if (int id = getNameID(name) > 0) {
    retStr.append(intToStr(id));
    return retStr;
  }

  bool hasid = false;
  do {
    int iidd = rand() % 99 + 1;
    if (mUsers.count(iidd) == 0) {
      auto pos = getNewPosition();
      mUsers.insert({ iidd, {name, pos} });
      retStr.append(intToStr(iidd));
      hasid = true;
    }
  } while (!hasid);

	return retStr;
}

std::string CServerData::msgAddMap(int id, std::string name, std::vector<std::pair<int, int>> points)
{
  std::string retStr("03/");
  mIslands.insert({ id, {name, points} });
  retStr += intToStr(id) + "/" + intToStr((int)points.size());
  return retStr;
}


std::string CServerData::msgGetMap()
{
	return std::string();
}


