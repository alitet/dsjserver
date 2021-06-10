#include "CServerData.h"
#include "Helpers.h"
#include <ctime>

typedef std::vector<std::string>::iterator vecStrIter;

class CServerData::dataRecord {
public:
  CServerData::dataRecord() 
    : nombre(""), personnummer(0), welcomeMsg("")
    , lastNumberOfIds(0), naveComp("")
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
  std::string naveComp;

};


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
  case 17: {
    int id = strToInt(tokens[1]);
    int radio = strToInt(tokens[2]);
    retStr = msgGetPlanetsInRadio(id, radio);
  } break;
  case 19: {
    int id = strToInt(tokens[1]);
    retStr = msgAddNave(id, tokens, 2);
  } break;
  case 21: {
    int id = strToInt(tokens[1]);
    int askId = strToInt(tokens[2]);
    retStr = msgGetNave(askId);
  } break;
  case 23: {
    int id = strToInt(tokens[1]);
    //int askId = strToInt(tokens[2]);
    //retStr = msgGetNave(askId);
  } break;
  case 25: {
    int id = strToInt(tokens[1]);
    int askId = strToInt(tokens[2]);
    //retStr = msgGetNave(askId);
  } break;
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
      if (val->position.x == 0 && 
          val->position.y == 0 && 
          val->position.z == 0) { continue; }

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

std::vector<int> CServerData::getIdsInRadio(int id, int radio)
{
  std::vector<int> retVec;
  auto rec = getRecord(id);
  for (auto& val : mvRecords) {
    if (val->personnummer == id) { continue; }
    float dist = distance(
      val->position.x, val->position.y, val->position.z,
      rec->position.x, rec->position.y, rec->position.z);
    if (dist < (val->diametro + rec->diametro)) {
      retVec.push_back(val->personnummer);
    }
  }
  return retVec;
}

std::string concatTokens(vecStrIter begin, vecStrIter end) 
{
  std::string retStr;
  for (auto it = begin; it != end; it++) {
    retStr.append(*it); retStr.append("-");
  }  
  return retStr;
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

std::string CServerData::msgGetPlanetsInRadio(int id, int radio)
{
  std::string retStr("018-");
  auto planets = getIdsInRadio(id, radio);
  retStr.append(intToStr(planets.size())); retStr.append("-");
  for (auto& val : planets) {
    retStr.append(intToStr(val)); retStr.append("-");
  }
  return retStr;
}

std::string CServerData::msgAddNave(int id, std::vector<std::string> tokens, int offset)
{
  std::string retStr("020-RECIBIDO");
  auto record = getRecord(id);
  if (record != nullptr) {
    auto begIt = tokens.begin() + offset;
    record->naveComp.assign(concatTokens(begIt, tokens.end()));
  }
  return retStr;
}

std::string CServerData::msgGetNave(int askId)
{
  std::string retStr("022-");
  auto record = getRecord(askId);
  if (record != nullptr) {
    if (record->naveComp != "") {
      retStr.append("01-");
      retStr.append(record->naveComp);
    }
    else {
      retStr.append("00");
    }
  }
  return retStr;
}

