#include "Constants.h"
#include <string>

namespace mDNSResolver {
  class Query {
  public:
    Query(std::string name);
    ~Query();
    unsigned char &getPacket();
    unsigned int length();

  private:
    unsigned char buffer[MDNS_MAX_NAME_LEN + 19];
    void assemblePacket(std::string &name);
    unsigned int packetLength;
    int buildDNSName(std::string &name, unsigned int bufferIndex);
  };
};