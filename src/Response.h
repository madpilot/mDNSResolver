#ifndef MDNS_RESOLVER_RESPONSE_H
#define MDNS_RESOLVER_RESPONSE_H

#include <IPAddress.h>

namespace mDNSResolver {
  class Response {
  public:
    Response();
    Response(const char* name);
    Response(const char* name, unsigned long ttl);
    ~Response();

    void decrementTTL();

    const char* name;
    long ttl;
    bool resolved;
    IPAddress ipAddress;
    Response* cname;

    bool operator==(const Response& response);
    bool operator!=(const Response& response);
  };
};

#endif
