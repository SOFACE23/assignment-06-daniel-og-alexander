//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) //takes local ip as an argument 
{
  try
  {
    if (argc != 2) //catch error
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context; //Provides core I/O functionality. 

    tcp::resolver resolver(io_context); 
    tcp::resolver::results_type endpoints = //takes the provided ip and links it to the resolver as an endpoint
      resolver.resolve(argv[1], "daytime");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints); //connects socket and the endpoint

    while(true)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error); //reads the data

      if (error == boost::asio::error::eof) //checks for errors
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len); //Writes the data
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}