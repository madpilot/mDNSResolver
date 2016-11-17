#ifndef MDNS_RESOLVER_QUERY_H
#define MDNS_RESOLVER_QUERY_H

#include "Constants.h"
#include <WiFiUdp.h>

#define MDNS_RESOLVER_MAX_CACHE 4

namespace mDNSResolver {
  class Query {
  public:
    Query(const char* name);
    ~Query();
    void sendPacket(WiFiUDP& socket);

  private:
    const char* name;
    void assemblePacket(unsigned char *buffer, int bufferLength);
    int buildDNSName(unsigned char *buffer, unsigned int bufferIndex);
  };
};

#endif
