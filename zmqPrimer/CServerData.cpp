#include "CServerData.h"
#include "Helpers.h"
#include <ctime>


class CServerData::dataRecord {
public:
  CServerData::dataRecord() 
    : nombre(""), personnummer(0), welcomeMsg("")
    , lastNumberOfIds(0)
  { }
  
  struct sPoint3 {
    int x; int y; int z;
    sPoint3() : x{ 0 }, y{ 0 }, z{ 0 } { }
  };

  std::string nombre;
  int personnummer;
  std::string welcomeMsg;
  int lastNumberOfIds;

  sPoint3 position;
  int diametro;
  std::string planetName;
};
//std::vector<std::pair<int, int>> getPointsInTokens(const std::vector<std::string>& tokens, int startIdx)
//{
//  std::vector<std::pair<int, int>> puntos;
//  int idx = 0;
//  for (int i = 0; i < strToInt(tokens[startIdx]); i++) {
//    int lx = strToInt(tokens[startIdx + 1 + idx]);
//    int ly = strToInt(tokens[startIdx + 1 + idx + 1]);
//    idx += 2;
//    puntos.push_back(std::make_pair(lx, ly));
//  }
//  return puntos;
//}
//
//std::vector<sWall> getWallsInTokens(const std::vector<std::string>& tokens, int startIdx)
//{
//  std::vector<sWall> walls;
//  int idx = 0;
//  for (int i = 0; i < strToInt(tokens[startIdx]); i++) {
//    int ti = strToInt(tokens[startIdx + 1 + idx]);
//    int lx = strToInt(tokens[startIdx + 1 + idx + 1]);
//    int ly = strToInt(tokens[startIdx + 1 + idx + 2]);
//    int lz = strToInt(tokens[startIdx + 1 + idx + 3]);
//    idx += 4;
//    
//    walls.push_back(sWall{ ti,{lx,ly,lz} });
//  }
//  return walls;
//}

std::string CServerData::comando(const std::vector<std::string> &tokens)
{
  std::string retStr = "error";
  int cmmd = strToInt(tokens[0]);

  switch (cmmd) {
  case 1:
    retStr = msgAddUser(tokens[1]);
    break;
  case 3: {
    int id = strToInt(tokens[1]);
    std::string welcomem = tokens[2];
    retStr = msgAddWelcome(welcomem, id);
  } break;
  case 5: {
    int id = strToInt(tokens[1]);
    retStr = msgAreNewIds(id);
  } break;
  case 7: {
    int id = strToInt(tokens[1]);    
		retStr = msgGetIds();
	} break;
	case 9: {
		int id = strToInt(tokens[1]);
    int askId = strToInt(tokens[2]);
		retStr = msgGetName(askId);
	} break;
	case 11: {
		int id = strToInt(tokens[1]);
    int askId = strToInt(tokens[2]);
    retStr = msgGetBienvenida(askId);
	} break;
	case 13: {
		int id = strToInt(tokens[1]);
    int diametro = strToInt(tokens[2]);
    std::string name = tokens[3];
    retStr = msgAddPlanet(id, diametro, name);		 
	} break;
	case 15: {
		int id = strToInt(tokens[1]);
    int askId = strToInt(tokens[1]);
		retStr = msgGetPlanet(askId);
	} break;
		//case 17: {
		//  int id = strToInt(tokens[1]);
		//  auto walls = getWallsInTokens(tokens, 2);
		//  retStr = msgAddWalls(id, walls);
		//} break;
		//}
	default: break;
  }
	return retStr;
}


int CServerData::getNameID(std::string name)
{
  for (const auto& val : mvRecords) {
    if (val->nombre == name) {
      return val->personnummer;
    }
  }
  return 0;
}

void CServerData::addNameID(std::string name, int id)
{
  auto record = std::make_shared<CServerData::dataRecord>();
  record->nombre = name; 
  record->personnummer = id;
  mvRecords.push_back(record);
}

bool CServerData::buscaID(int id)
{
  for (const auto& val : mvRecords) {
    if (val->personnummer == id) { return true; }
  }
  return false;
}

std::shared_ptr<CServerData::dataRecord> CServerData::getRecord(int id)
{
  for (auto& val : mvRecords) {
    if (val->personnummer == id) { return val; }
  }
  return nullptr;
}

int CServerData::IDCount()
{  
  return static_cast<int>(mvRecords.size());
}

std::tuple<int, int, int> CServerData::findNewPosition()
{
  bool haspos = false;
  std::tuple<int, int, int> retPos;

  do {
    int xx = rand() % 950;
    int yy = rand() % 950;
    int zz = rand() % 950;
    
    bool found = true;
    for (const auto& val : mvRecords) {
      int xval = val->position.x;
      int yval = val->position.y;
      int zval = val->position.z;
      if (distance(xx, yy, zz, xval, yval, zval) < 90.f) {
        found = false;
        break;
      }
    }
    if (found) {
      retPos = std::make_tuple(xx, yy, zz);
      haspos = true;
    }
  } while (!haspos);
  return retPos;
}

//---------------------------------------


std::string CServerData::msgAddUser(std::string name)
{
  std::string retStr("002-");
  int id = getNameID(name);
  if (id > 0) {
    retStr.append("RECIBIDO-");
    retStr.append(intToStr(id));
    return retStr;
  }

  bool hasid = false;
  do {
    srand((int)time(0));
    int iidd = rand() % 99 + 1;
    if (!buscaID(iidd)) {
      addNameID(name, iidd);
      retStr.append("RECIBIDO-");
      retStr.append(intToStr(iidd));
      hasid = true;
    }
  } while (!hasid);
	return retStr;
}

std::string CServerData::msgAddWelcome(std::string msg, int id)
{
  std::string retStr("004-RECIBIDO");
  auto record = getRecord(id);
  if (record != nullptr) {    
    record->welcomeMsg.append(msg);
  }
  return retStr;
}

std::string CServerData::msgAreNewIds(int id)
{
  std::string retStr("006-");
  auto record = getRecord(id);
  int count = IDCount();
  if (count > record->lastNumberOfIds) {
    record->lastNumberOfIds = count;
    retStr.append("01");
  }
  else {
    retStr.append("00");
  }
  return retStr;
}


std::string CServerData::msgGetIds()
{
  std::string retStr("008-");
  retStr.append(intToStr((int)mvRecords.size())); retStr.append("-");

  for (const auto& val : mvRecords) {
    retStr.append(intToStr(val->personnummer)); retStr.append("-");
  }
  return retStr;
}

std::string CServerData::msgGetName(int askid)
{
  std::string retStr("010-");
  auto record = getRecord(askid);
  if (record != nullptr) {
    retStr.append(record->nombre);
  }
  return retStr;
}

std::string CServerData::msgGetBienvenida(int askid)
{
  std::string retStr("012-");
  auto record = getRecord(askid);
  if (record != nullptr) {
    retStr.append(record->welcomeMsg);
  }
  return retStr;
}

std::string CServerData::msgAddPlanet(int id, int diametro, const std::string &name)
{
  std::string retStr("014-");
  auto record = getRecord(id);
  int xp = 0, yp = 0, zp = 0;
  if (record != nullptr) {
    record->diametro = diametro;
    record->planetName = name;
    std::tie(xp, yp, zp) = findNewPosition();
    record->position.x = xp; 
    record->position.y = yp;
    record->position.z = zp;
    retStr.append(intToStr(xp)); retStr.append("-");
    retStr.append(intToStr(yp)); retStr.append("-");
    retStr.append(intToStr(zp));
  }
  return retStr;
}

std::string CServerData::msgGetPlanet(int askid)
{
  std::string retStr("016-");
  auto record = getRecord(askid);  
  if (record != nullptr) {
    if (record->position.x != 0 ||
      record->position.y != 0 ||
      record->position.z != 0) {
      retStr.append("01-");
      retStr.append(intToStr(record->position.x)); retStr.append("-");
      retStr.append(intToStr(record->position.y)); retStr.append("-");
      retStr.append(intToStr(record->position.z)); retStr.append("-");
      retStr.append(intToStr(record->diametro));
    }
    else {
      retStr.append("00");
    }
  }
  return retStr;
}



//std::string CServerData::msgAddMap(int id, std::string name, std::vector<std::pair<int, int>> points)
//{
//  std::string retStr("03/");
//  if (mUsers.count(id) == 0) { 
//    retStr += intToStr(id) + "/0";
//    return retStr; 
//  }
//
//  auto pos = getNewPosition();
//  mIslands.insert({ id, {name, pos, points} });
//  retStr += intToStr(id) + "/" + intToStr((int)points.size());
//  return retStr;
//}
//
//std::string CServerData::msgNumIds()
//{
//  std::string retStr("06/");
//  retStr += intToStr(mUsers.size());
//
//  for (const auto &val : mUsers) {
//    retStr += "/" + intToStr(val.first);
//  }
//  return retStr;
//}
//
//std::string CServerData::msgGetIslandData(int id)
//{
//  std::string retStr("08/");
//  if (mIslands.count(id) == 0) {
//    retStr += "none";
//    return retStr;
//  }
//
//  auto isla = mIslands[id];
//  retStr += isla.name + "/" + intToStr(isla.pos.first) + 
//    "/" + intToStr(isla.pos.second);
//  return retStr;
//}
//
//std::string CServerData::msgGetIslandPoints(int id)
//{
//  std::string retStr("10/");
//  if (mIslands.count(id) == 0) {
//    retStr += "0";
//    return retStr;
//  }
//  auto isla = mIslands[id];
//  retStr += intToStr(isla.points.size());
//  for (const auto& val : isla.points) {
//    retStr += "/" + intToStr(val.first) + "/" + intToStr(val.second);
//  }    
//  return retStr;
//}
//
//std::string CServerData::msgGetUserName(int id)
//{
//  std::string retStr("12/");
//  retStr += mUsers.count(id) == 0 ? "none" : mUsers[id];  
//  return retStr;
//}
//
//std::string CServerData::msgAddTrees(int id, std::vector<std::pair<int, int>> trees)
//{
//  std::string retStr("14/");
//  if (mUsers.count(id) == 0) {
//    retStr += intToStr(id) + "/0";
//    return retStr;
//  }
//
//  mTrees.insert({ id, {trees} });
//
//  retStr += intToStr(id) + "/" + intToStr((int)trees.size());
//  return retStr;
//}
//
//std::string CServerData::msgAddWalls(int id, std::vector<sWall> walls)
//{
//  std::string retStr("18/");
//  if (mUsers.count(id) == 0) {
//    retStr += intToStr(id) + "/0";
//    return retStr;
//  }
//
//  mWalls.insert({ id, {walls} });
//
//  retStr += intToStr(id) + "/" + intToStr((int)walls.size());
//  return retStr;
//}
//
//std::string CServerData::msgGetTrees(int id)
//{
//  std::string retStr("16/");
//  if (mTrees.count(id) == 0) {
//    retStr += "none";
//    return retStr;
//  }
//
//  auto tree = mTrees[id];
//  retStr += intToStr(tree.size());
//  for (const auto& val : tree) {
//    retStr += "/" + intToStr(val.first) + "/" + intToStr(val.second);
//  }
//  return retStr;
//}

