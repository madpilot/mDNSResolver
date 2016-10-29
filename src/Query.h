#ifndef MDNS_RESOLVER_QUERY_H
#define MDNS_RESOLVER_QUERY_H

#include "Constants.h"
#include <string>
#include "../mocks/UDP.h"

#define MDNS_RESOLVER_MAX_CACHE 4

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

#endif
