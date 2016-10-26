#include "Constants.h"
#include <string>
#include "../mocks/UDP.h"

namespace mDNSResolver {
  class Query {
  public:
    Query(std::string name);
    ~Query();
    void sendPacket(UDP *socket);

  private:
    std::string name;
    void assemblePacket(unsigned char *buffer, int bufferLength);
    int buildDNSName(unsigned char *buffer, unsigned int bufferIndex);
  };
};