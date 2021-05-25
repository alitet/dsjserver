#include "zmq.hpp"
#include "CServerData.h"
#include "Helpers.h"
#include <conio.h>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

int main()
{
  zmq::context_t context{ 1 };

  zmq::socket_t socket{ context, zmq::socket_type::rep };
  socket.bind("tcp://*:5555");

  //const std::string data{ "02/01" };
  int ch = 0;

  CServerData serverData;
  std::vector<std::string> fakeUser1{ "001", "Reed" };
  std::vector<std::string> fakeUser2{ "001", "Sue" };
  std::vector<std::string> fakeUser3{ "001", "Johnny" };
  std::vector<std::string> fakeUser4{ "001", "Ben" };
  
  auto id1 = tokenizer(serverData.comando(fakeUser1),'-').at(2);
  auto id2 = tokenizer(serverData.comando(fakeUser2),'-').at(2);
  auto id3 = tokenizer(serverData.comando(fakeUser3),'-').at(2);
  auto id4 = tokenizer(serverData.comando(fakeUser4),'-').at(2);

  std::vector<std::string> fakePlanet1{ "013", id1, "40", "elastic" };
  std::vector<std::string> fakePlanet2{ "013", id2, "50", "invisible" };
  std::vector<std::string> fakePlanet3{ "013", id3, "60", "flames" };
  std::vector<std::string> fakePlanet4{ "013", id4, "70", "Dwayne" };

  serverData.comando(fakePlanet1);
  serverData.comando(fakePlanet2);
  serverData.comando(fakePlanet3);
  serverData.comando(fakePlanet4);

  //serverData.addFakeUsers();
  //serverData.addFakeIslands();
  //serverData.addFakeTrees();

  //do
  while(true)
  {
    zmq::message_t request;

    socket.recv(request, zmq::recv_flags::none);
    std::string reqMsg = request.to_string();
    std::cout << "Recibi: " << reqMsg << std::endl;
    
    std::this_thread::sleep_for(200ms);
    
    auto tokens = tokenizer(reqMsg, '-');
    std::string resp = serverData.comando(tokens);

    socket.send(zmq::buffer(resp), zmq::send_flags::none);

    //ch = toupper(_getch());    
  } //while (ch != 'X');

  socket.close();
	return 0;
}