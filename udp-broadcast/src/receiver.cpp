#include <atomic>
#include <chrono>
#include <thread>
#include <csignal>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace std::chrono;

int main() {
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(13337);
  address.sin_addr.s_addr = INADDR_ANY;

  const int broadcast = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
    printf("Error in setting SO_BROADCAST option\n");
    return 1;
  }

  if (bind(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
    printf("Error in binding socket to 255.255.255.255:13337\n");
    return 1;
  }

  while (true) {
    sockaddr_in sender_address{};
    socklen_t addr_length{};

    char buffer[50] = {};
    recvfrom(sockfd, &buffer[0], sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&sender_address), &addr_length);

    char sender_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sender_address.sin_addr), sender_ip, sizeof(sender_ip));

    printf("received '%s' message from '%s'\n", buffer, sender_ip);

    std::this_thread::sleep_for(1s);
  }
}