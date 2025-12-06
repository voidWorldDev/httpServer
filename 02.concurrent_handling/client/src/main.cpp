#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  const char *SERVER_IP = "127.0.0.1";
  const int PORT = 65432;

  // 1. Create socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "socket() failed\n";
    return 1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

  // 2. Connect to server
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "connect() failed — is server running?\n";
    close(sock);
    return 1;
  }

  std::cout << "Connected to server! Type messages (or 'quit' to exit):\n";

  std::string message;
  char buffer[1024];

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, message);

    if (message == "quit")
      break;
    if (message.empty())
      continue;

    // Send message
    send(sock, message.c_str(), message.size(), 0);

    // Receive echo
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
      std::cout << "Server disconnected.\n";
      break;
    }

    buffer[bytes_received] = '\0';
    std::cout << "Echo: " << buffer << "\n";
  }

  close(sock);
  std::cout << "Goodbye!\n";
  return 0;
}
