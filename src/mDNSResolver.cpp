#include "mDNSResolver.h"
#include <millis.h>
#include <assert.h>

namespace mDNSResolver {
  Cache cache;

  Resolver::Resolver(UDP udp) {
    timeout = 0;
    found = false;
    this->udp = udp;
    this->localIP = IPAddress(127, 0, 0, 1);
  }

  Resolver::Resolver(UDP udp, IPAddress localIP) {
    timeout = 0;
    found = false;
    this->udp = udp;
    this->localIP = localIP;
  }

  Resolver::~Resolver() {}

  void Resolver::setLocalIP(IPAddress localIP) {
    this->localIP = localIP;
  }

  bool Resolver::search(std::string name) {
    cache.expire();

    int attempts = 0;
    found = false;

    int index = cache.search(name);
    if(index == -1) {
      cache.insert(Response(name));
    } else if(cache[index].resolved) {
      found = true;
      lastIPAddress = cache[index].ipAddress;
      return true;
    }

    while(attempts < MDNS_ATTEMPTS) {
      long now = millis();

      // Send a query packet every second
      if(now - timeout > MDNS_RETRY) {
        query(name);
        timeout = now;
        attempts++;
      }

      loop();

      index = cache.search(name);

      if(index != -1 && cache[index].resolved) {
        found = true;
        lastIPAddress = cache[index].ipAddress;
        return true;
      }
    }

    return false;
  }

  IPAddress Resolver::address() {
    if(found) {
      return lastIPAddress;
    } else {
      assert("search() must be called first, and must have returned true.");
    }
  }

  void Resolver::query(std::string& name) {
    Query query(name);
    udp.beginPacketMulticast(MDNS_BROADCAST_IP, MDNS_PORT, localIP, UDP_TIMEOUT);
    query.sendPacket(&udp);
    udp.endPacket();
  }

  void Resolver::loop() {
    cache.expire();

    unsigned int len = udp.parsePacket();
    if(len > 0) {
      if(!init) {
        init = true;
        udp.beginMulticast(localIP, MDNS_BROADCAST_IP, MDNS_PORT);
      }

      unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * len);
      udp.read(buffer, len);
      Answer::process(buffer, len, cache);
      free(buffer);
    }
  }
};
