#ifndef MDNS_RESOLVER_h
#define MDNS_RESOLVER_h

#include <IPAddress.h>
#include <UDP.h>
#include "Cache.h"
#include "Query.h"
#include "Answer.h"

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
      Resolver(UDP udp, IPAddress localIP);
      ~Resolver();

      std::string resolve(std::string name);
      void setLocalIP(IPAddress localIP);
      bool search(std::string name);
      IPAddress address();
    private:
      UDP udp;
      IPAddress localIP;
      void loop();
      bool found;
      bool init;
      long timeout;
      void query(std::string &name);
      IPAddress lastIPAddress;
  };
};
#endif
