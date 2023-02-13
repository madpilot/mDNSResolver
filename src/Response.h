#ifndef MDNS_RESOLVER_RESPONSE_H
#define MDNS_RESOLVER_RESPONSE_H

#include "Constants.h"

#ifdef ARDUINO_ARCH_RP2040
#include <WiFi.h>
#else
#include <IPAddress.h>
#endif


namespace mDNSResolver {
  class Response {
  public:
    Response();
    Response(const char* name);
    Response(const char* name, unsigned long ttl);
    ~Response();

    void decrementTTL();

    char name[MDNS_MAX_NAME_LEN + 1];
    long ttl;
    bool resolved;
    IPAddress ipAddress;
    Response* cname;

    bool operator==(const Response& response);
    bool operator!=(const Response& response);
  };
};

#endif
