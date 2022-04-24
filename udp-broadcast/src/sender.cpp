#include <atomic>
#include <chrono>
#include <thread>
#include <cassert>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace std::chrono;

class Finally final {
 public:
  using F = std::function<void()>;

  Finally(F f) : f_{std::move(f)} {
    assert(f_ && "f_ must me set");
  }

  ~Finally() {
    if (!f_) {
      return;
    }

    f_();
  }

 private:
  F f_;
};

int main() {
  auto sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  Finally finalizer{
    [&sockfd]() {
      close(sockfd);
    }
  };

  const int broadcast = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
    printf("Error in setting SO_BROADCAST option\n");
    return 1;
  }

  sockaddr_in address{};
  address.sin_port = htons(13337);
  address.sin_addr.s_addr = INADDR_BROADCAST;

  while (true) {
    const char* message = "broadcast hello";
    sendto(sockfd, message, std::strlen(message) + 1, 0, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    printf("sent '%s' message\n", message);
    std::this_thread::sleep_for(1s);
  }
}