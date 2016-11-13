#ifndef MDNS_RESOLVER_h
#define MDNS_RESOLVER_h

#include <IPAddress.h>
#include <UDP.h>
#include "Cache.h"

namespace mDNSResolver {
  class Resolver {
    public:
      Resolver();
      ~Resolver();
      IPAddress search(std::string name);

    private:
      void loop();
  };
};
#endif
