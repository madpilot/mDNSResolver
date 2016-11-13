#include "mDNSResolver.h"

namespace mDNSResolver {
  Cache cache;

  Resolver::Resolver(UDP udp) {
    timeout = 0;
    attempts = 0;
    found = false;
    this->udp = udp;
  }

  Resolver::~Resolver() {}

  bool Resolver::search(std::string name) {
    cache.expire();

    long now = millis();
    attempts = 0;
    found = false;

    int index = cache.search(name);

    while(attempts < MDNS_ATTEMPTS) {
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

  IPAddress Resolved::address() {
    if(found) {
      return lastIPAddress;
    } else {
      assert("search() must be called first, and must have returned true.");
    }
  }

  void Resolver::query(std::string& name) {
    Query query(name);
    udp.beginPacketMulticast(MDNS_BROADCAST_IP, MDNS_PORT, WiFi.localIP(), UDP_TIMEOUT);
    query.sendPacket(udp);
    udp.endPacket();
  }

  void Resolver::loop() {
    cache.expire();

    unsigned int len = Udp.parsePacket();
    if(len > 0) {
      if(!init) {
        init = true;
        udp.beginMulticast(WiFi.localIP(), MDNS_BROADCAST_IP, MDNS_PORT);
      }

      byte *buffer = (byte *)malloc(sizeof(byte) * len);
      Udp.read(buffer, len);
      Answer.process(buffer, len, &cache);
      free(buffer);
    }
  }
};
