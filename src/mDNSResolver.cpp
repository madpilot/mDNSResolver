#include "mDNSResolver.h"
#include "Arduino.h"
#include <assert.h>

namespace mDNSResolver {
  Cache cache;

  Resolver::Resolver(WiFiUDP& udp) {
    timeout = 0;
    this->udp = udp;
  }

  Resolver::Resolver(WiFiUDP& udp, IPAddress localIP) {
    timeout = 0;
    this->udp = udp;
    this->localIP = localIP;
  }

  Resolver::~Resolver() {}

  void Resolver::setLocalIP(IPAddress localIP) {
    this->localIP = localIP;
  }

  IPAddress Resolver::search(std::string name) {
    cache.expire();

    int attempts = 0;

    while(attempts < MDNS_ATTEMPTS) {
      int index = cache.search(name);

      if(index == -1) {
        cache.insert(Response(name, 5));
        continue;
      } else if(cache[index].resolved) {
        return cache[index].ipAddress;
      }

      long now = millis();

      // Send a query packet every second
      if(now - timeout > MDNS_RETRY) {
        query(name);
        timeout = now;
        attempts++;
      }

      loop();
    }

    return INADDR_NONE;
  }

  void Resolver::query(std::string& name) {
    Query query(name);
    udp.beginPacketMulticast(MDNS_BROADCAST_IP, MDNS_PORT, localIP, UDP_TIMEOUT);
    query.sendPacket(udp);
    udp.endPacket();
  }

  void Resolver::loop() {
    cache.expire();

    if(!init) {
      init = true;
      udp.beginMulticast(localIP, MDNS_BROADCAST_IP, MDNS_PORT);
    }

    unsigned int len = udp.parsePacket();
    if(len > 0) {
      unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * len);

      if(buffer == NULL) {
        // Out of memory - the packet is probably too big to parse. Probably.
        // Silently bombing out, possibly isn'te great, but it'll do for the moment.
        return;
      }

      udp.read(buffer, len);
      Answer::process(buffer, len, cache);
      free(buffer);
    }
  }
};
