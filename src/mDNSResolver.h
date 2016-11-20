#ifndef MDNS_RESOLVER_h
#define MDNS_RESOLVER_h

#include <IPAddress.h>
#include <WiFiUdp.h>
#include "Cache.h"
#include "Query.h"
#include "Answer.h"

#define MDNS_BROADCAST_IP IPAddress(224, 0, 0, 251)
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

#ifndef MDNS_TLD
#define MDNS_TLD ".local"
#endif

namespace mDNSResolver {
  class Resolver {
    public:
      Resolver(WiFiUDP& udp);
      Resolver(WiFiUDP& udp, IPAddress localIP);
      ~Resolver();
			void setLocalIP(IPAddress localIP);
      IPAddress search(const char* name);
      MDNS_RESULT lastResult;
      void loop();
    private:
      WiFiUDP udp;
      IPAddress localIP;
      MDNS_RESULT read();
      bool init;
      long timeout;
      void query(const char* name);
      bool isMDNSName(const char* name);
  };
};
#endif
