#ifndef MDNS_RESOLVER_h
#define MDNS_RESOLVER_h

#include <IPAddress.h>
#include <WiFi.h>
#include <UDP.h>
#include "Cache.h"

#define MDNS_BROADCAST_IP IPAddress(255, 0, 0, 251)
#define MDNS_PORT         5353

#ifndef MDNS_RETRY
#define MDNS_RETRY        1000
#endif

#ifndef MDNS_ATTEMPTS
#define MDNS_ATTEMPTS     5
#endif


#ifndef UDP_TIMEOUT
#define UDP_TIMEOUT       255
#endif

namespace mDNSResolver {
  class Resolver {
    public:
      Resolver(UDP udp);
      ~Resolver();

      bool search(std::string name);
      IPAdress address();
    private:
      UDP udp;
      IPAddress localIP;
      void loop();
      bool init;
      long timeout;
      int attempts;
      void query(std::string &name);
      IPAddress lastIPAddress;
  };
};
#endif
