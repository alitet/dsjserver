#include "CServerData.h"
#include "Helpers.h"


std::vector<std::pair<int, int>> getPointsInTokens(const std::vector<std::string>& tokens, int startIdx)
{
  std::vector<std::pair<int, int>> puntos;
  int idx = 0;
  for (int i = 0; i < strToInt(tokens[startIdx]); i++) {
    int lx = strToInt(tokens[startIdx + 1 + idx]);
    int ly = strToInt(tokens[startIdx + 1 + idx + 1]);
    idx += 2;
    puntos.push_back(std::make_pair(lx, ly));
  }
  return puntos;
}

std::vector<sWall> getWallsInTokens(const std::vector<std::string>& tokens, int startIdx)
{
  std::vector<sWall> walls;
  int idx = 0;
  for (int i = 0; i < strToInt(tokens[startIdx]); i++) {
    int ti = strToInt(tokens[startIdx + 1 + idx]);
    int lx = strToInt(tokens[startIdx + 1 + idx + 1]);
    int ly = strToInt(tokens[startIdx + 1 + idx + 2]);
    int lz = strToInt(tokens[startIdx + 1 + idx + 3]);
    idx += 4;
    
    walls.push_back(sWall{ ti,{lx,ly,lz} });
  }
  return walls;
}

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

    auto puntos = getPointsInTokens(tokens, 4);
    retStr = msgAddMap(id, name, puntos);

  } break;
  case 5:
    retStr = msgNumIds();
    break;
  case 7: {
    int id = strToInt(tokens[1]);
    retStr = msgGetIslandData(id);
  } break;
  case 9: {
    int id = strToInt(tokens[1]);
    retStr = msgGetIslandPoints(id);
  } break;
  case 11: {
    int id = strToInt(tokens[1]);
    retStr = msgGetUserName(id);
  } break;
  default: break;
  case 13: {
    int id = strToInt(tokens[1]);    
    auto trees = getPointsInTokens(tokens, 2);
    retStr = msgAddTrees(id, trees);

  } break;
  case 15: {
    int id = strToInt(tokens[1]);
    retStr = msgGetTrees(id);
  } break;
  case 17: {
    int id = strToInt(tokens[1]);
    auto walls = getWallsInTokens(tokens, 2);
    retStr = msgAddWalls(id, walls);
  } break;
  }
	return retStr;
}

void CServerData::addFakeUsers()
{
  mUsers.insert({ 67, "beckham"}); 
  mUsers.insert({ 48, "akira"  }); 
  mUsers.insert({ 12, "fidel"  }); 
  mUsers.insert({ 81, "mahalo" }); 
}

void CServerData::addFakeIslands()
{
  mIslands.insert({ 67, 
    { "england", std::make_pair(232, 527) ,{
      std::make_pair(15, 10), std::make_pair(20, 5), std::make_pair(30, 5),
      std::make_pair(35, 15), std::make_pair(30, 25), std::make_pair(35, 45),
      std::make_pair(10, 45), std::make_pair(15, 30)
    }}});

  mIslands.insert({ 48,
    { "japan", std::make_pair(143, 628) ,{
      std::make_pair(25, 10), std::make_pair(35, 10), std::make_pair(35, 30),
      std::make_pair(15, 45), std::make_pair(10, 35)
    }} });

  mIslands.insert({ 12,
    { "cuba", std::make_pair(74, 455) ,{
      std::make_pair(5, 25), std::make_pair(20, 15), std::make_pair(40, 15),
      std::make_pair(45, 30)
    }} });

  mIslands.insert({ 81,
    { "hawaii", std::make_pair(14, 24) ,{
      std::make_pair(15, 10), std::make_pair(35, 20), std::make_pair(40, 30),
      std::make_pair(20, 40), std::make_pair(5, 25)
    }} });

}

void CServerData::addFakeTrees()
{
  mTrees.insert({ 67, {std::make_pair(8, 38), std::make_pair(23, 40)} });
  mTrees.insert({ 48, {std::make_pair(3, 16), std::make_pair(18, 48)} });
  mTrees.insert({ 12, {std::make_pair(40, 49), std::make_pair(28,44)} });
  mTrees.insert({ 81, {std::make_pair(9, 15), std::make_pair(15, 43)} });
    
}

std::pair<int, int> CServerData::getNewPosition()
{
  bool haspos = false;
  std::pair<int, int> retPos;

  do {
    int xx = rand() % 950;
    int yy = rand() % 950;
    bool found = true;
    for (const auto& val : mIslands) {
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
    if (val.second == name) {
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
      mUsers.insert({ iidd, name });
      retStr.append(intToStr(iidd));
      hasid = true;
    }
  } while (!hasid);

	return retStr;
}

std::string CServerData::msgAddMap(int id, std::string name, std::vector<std::pair<int, int>> points)
{
  std::string retStr("03/");
  if (mUsers.count(id) == 0) { 
    retStr += intToStr(id) + "/0";
    return retStr; 
  }

  auto pos = getNewPosition();
  mIslands.insert({ id, {name, pos, points} });
  retStr += intToStr(id) + "/" + intToStr((int)points.size());
  return retStr;
}

std::string CServerData::msgNumIds()
{
  std::string retStr("06/");
  retStr += intToStr(mUsers.size());

  for (const auto &val : mUsers) {
    retStr += "/" + intToStr(val.first);
  }
  return retStr;
}

std::string CServerData::msgGetIslandData(int id)
{
  std::string retStr("08/");
  if (mIslands.count(id) == 0) {
    retStr += "none";
    return retStr;
  }

  auto isla = mIslands[id];
  retStr += isla.name + "/" + intToStr(isla.pos.first) + 
    "/" + intToStr(isla.pos.second);
  return retStr;
}

std::string CServerData::msgGetIslandPoints(int id)
{
  std::string retStr("10/");
  if (mIslands.count(id) == 0) {
    retStr += "0";
    return retStr;
  }
  auto isla = mIslands[id];
  retStr += intToStr(isla.points.size());
  for (const auto& val : isla.points) {
    retStr += "/" + intToStr(val.first) + "/" + intToStr(val.second);
  }    
  return retStr;
}

std::string CServerData::msgGetUserName(int id)
{
  std::string retStr("12/");
  retStr += mUsers.count(id) == 0 ? "none" : mUsers[id];  
  return retStr;
}

std::string CServerData::msgAddTrees(int id, std::vector<std::pair<int, int>> trees)
{
  std::string retStr("14/");
  if (mUsers.count(id) == 0) {
    retStr += intToStr(id) + "/0";
    return retStr;
  }

  mTrees.insert({ id, {trees} });

  retStr += intToStr(id) + "/" + intToStr((int)trees.size());
  return retStr;
}

std::string CServerData::msgAddWalls(int id, std::vector<sWall> walls)
{
  std::string retStr("18/");
  if (mUsers.count(id) == 0) {
    retStr += intToStr(id) + "/0";
    return retStr;
  }

  mWalls.insert({ id, {walls} });

  retStr += intToStr(id) + "/" + intToStr((int)walls.size());
  return retStr;
}

std::string CServerData::msgGetTrees(int id)
{
  std::string retStr("16/");
  if (mTrees.count(id) == 0) {
    retStr += "none";
    return retStr;
  }

  auto tree = mTrees[id];
  retStr += intToStr(tree.size());
  for (const auto& val : tree) {
    retStr += "/" + intToStr(val.first) + "/" + intToStr(val.second);
  }
  return retStr;
}

