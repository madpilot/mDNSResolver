#ifndef MDNS_RESOLVER_RESPONSE_H
#define MDNS_RESOLVER_RESPONSE_H

#include <string>
#include <IPAddress.h>

namespace mDNSResolver {
  class Response {
  public:
    Response();
    Response(std::string name);
    Response(std::string name, unsigned long ttl);
    ~Response();

    void decrementTTL();

    std::string name;
    long ttl;
    bool resolved;
    IPAddress ipAddress;
    Response* cname;

    bool operator==(const Response& response);
  };
};

#endif
