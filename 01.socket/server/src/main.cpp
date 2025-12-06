#include <arpa/inet.h>
#include_next <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  const int PORT = 65432;

  //--------------------------------------------------------------------
  // server_fd -> gets return with the file discriptor 0,1,2
  // other thing in place can be used -> go to documentatin for this
  // as for this
  // AF_INET - IPv4
  // SOCK_STREAM - TCP connection
  // 0 - tells to handle the tcp connection
  //
  // similarly SOCK_DGRAM for UDP connection
  //
  // for similar want to send and use the binary packages instead
  // can replace SOCK_STREAM to SOCK_RAW
  //
  // socket (ADDRESS_FAMILY, SOCKET_TYPES, PROTOCOL)
  //
  // return the file discriptor, negative if not successful
  //------------------------------------------------------------------

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "file discriptor or socket() failed";
    return 1;
  }

  //----------------------------------------------------------------------
  // Binding the socket -> essentially telling that the socket should
  // listen to the port Y of IP X.
  //
  // 1. get the struct of IPv4 address and put all the values null/0.
  //      can be done using address{}; or address={};
  // 2. Set the address family (IPv4)
  // 3. Connection to all available interface (localhost, LAN IP etc...)
  // 4. Set the port number to PORT.
  //
  // use bind(sock_file_discriptor, sockaddr (genric/base type), len of
  // sockaddr)
  // don't forget to close the connection via close(server_fd)
  //-----------------------------------------------------------------------

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // here s_addr -> raw IPv4 address
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "bind() failed\n";
    close(server_fd);
    return 1;
  }

  //--------------------------------------------------------------------------------
  // Finally listing to the connection
  // listen takes the 2 parameters to it listen (socket_fd, int backlog)
  // backlog -> allow only "backlog" number of incoming connection to completed
  // queue what to do with other? reject it, delay it or drop it depending upon
  // the network and os config so it is nothing to do with the incomplete queue
  // doing it with TCP handshake
  //-------------------------------------------------------------------------------

  if (listen(server_fd, 3) < 0) {
    std::cerr << "listen() failed\n";
    close(server_fd);
    return 1;
  }

  std::cout << "-------------------------------------------------" << "\n";
  std::cout << "        server on PORT " << PORT << "\n";
  std::cout << "-------------------------------------------------" << "\n";

  while (true) {
    socklen_t addr_len = sizeof(address);

    int client_fd = accept(server_fd, (struct sockaddr *)&address, &addr_len);
    // client_fd requrires last parameter as pointer not as value

    if (client_fd < 0) {
      std::cerr << "accept() getting the value of client_fd failed";
      continue; // don't want thing to fail silently...
    }

    //------------------------------------------------------
    // will use the 'inet_ntop' to read the binary data to
    // the human readable form otherwise networking can be
    // done without just for managing logs
    //------------------------------------------------------

    char client_ip[INET_ADDRSTRLEN];

    // inet_ntop structure
    // inet_ntop(int af, const void *__restrict cp, char *__restrict buf,
    // socklen_t len)

    inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << "Client address" << client_ip << "\n";

    char buffer[1024];
    while (true) {
      int byte_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

      if (byte_received <= 0) {
        if (byte_received == 0) {
          std::cout << "client disconnected. \n";
        } else {
          std::cerr << "recv() failed. \n";
        }
        break;
      }

      buffer[byte_received] = '\0';
      std::cout << "Received: " << buffer;

      send(client_fd, buffer, byte_received, 0);
    }
    close(client_fd);
    std::cout << "-------------------------------------------------" << "\n";
    std::cout << "Waiting for the next client" << "\n";
    std::cout << "-------------------------------------------------" << "\n";
  }
  close(server_fd);
  return 0;
}
