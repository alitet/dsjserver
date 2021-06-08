#pragma once
#include <unordered_map>
#include <utility>
#include <string>
#include <memory>

class CServerData
{
private:
  class dataRecord;
  std::vector<std::shared_ptr<dataRecord>> mvRecords;


public:
  std::string comando(const std::vector<std::string> &tokens);

private:
  std::string msgAddUser(std::string name);
  std::string msgAddWelcome(std::string msg, int id);
  std::string msgAreNewIds(int id);
  std::string msgGetIds();
  std::string msgGetName(int askid);
  std::string msgGetBienvenida(int askid);
  std::string msgAddPlanet(int id, int diametro, const std::string& name);
  std::string msgGetPlanet(int askid);
  std::string msgGetPlanetsInRadio(int id, int radio);
  std::string msgAddNave(int id, std::vector<std::string> tokens, int offset);
  std::string msgGetNave(int askId);

  //std::string msgAddMap(int id, std::string name, std::vector<std::pair<int, int>> points);
  //std::string msgNumIds();
  //std::string msgGetIslandData(int id);
  //std::string msgGetIslandPoints(int id);
  //std::string msgGetUserName(int id);
  //std::string msgGetTrees(int id);

  //std::string msgAddTrees(int id, std::vector<std::pair<int, int>> trees);
  //std::string msgAddWalls(int id, std::vector<sWall> walls);

  //std::string msgGetMap();

  //std::pair<int, int> getNewPosition();
  int getNameID(std::string name);
  void addNameID(std::string name, int id);

  bool buscaID(int id);
  int IDCount();

  std::tuple<int, int, int> findNewPosition();

  std::shared_ptr<dataRecord> getRecord(int id);
  std::vector<int> getIdsInRadio(int id, int radio);
  


};

