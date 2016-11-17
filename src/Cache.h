#ifndef MDNS_RESOLVER_CACHE_H
#define MDNS_RESOLVER_CACHE_H

#include "Constants.h"
#include "Response.h"

namespace mDNSResolver {
  class Cache {
  public:
    Cache();
    ~Cache();
    void insert(Response response);
    void remove(Response& response);

    int search(Response& response);
    int search(const char* name);

    void expire();
    int length();
    Response& operator[] (const int index);
  private:
    int oldest();
    void remove(int index);
    void removeCname(int index);

    Response list[MDNS_RESOLVER_MAX_CACHE];
    int size;
  };
};

#endif
