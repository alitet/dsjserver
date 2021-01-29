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
  
  const std::string data{ "02/01" };
  //int ch = 0;

  CServerData serverData;

  //do
  while(true)
  {
    zmq::message_t request;

    socket.recv(request, zmq::recv_flags::none);
    std::string reqMsg = request.to_string();
    std::cout << "Received: " << reqMsg << std::endl;
    
    std::this_thread::sleep_for(200ms);
    
    auto tokens = tokenizer(reqMsg, '/');
    auto resp = serverData.comando(tokens);

    socket.send(zmq::buffer(resp), zmq::send_flags::none);

    //ch = toupper(_getch());    
  } //while (ch != 'X');  

	return 0;
}