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


std::string CServerData::msgGetMap()
{
	return std::string();
}


