#ifndef MDNS_RESOLVER_RESPONSE_H
#define MDNS_RESOLVER_RESPONSE_H

#include <string>

namespace mDNSResolver {
  class Response {
  public:
    Response();
    Response(std::string name);
    Response(std::string name, unsigned long ttl);
    ~Response();
    void decrementTTL();
    long ttl;
    std::string name;
    bool operator==(const Response& response);
  };
};

#endif
