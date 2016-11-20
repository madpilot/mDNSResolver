#include "mDNSResolver.h"
#include "Arduino.h"

#ifdef TEST
#include <stdlib.h>
#endif

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

  IPAddress Resolver::search(const char* name) {
    if(!isMDNSName(name)) {
      return INADDR_NONE;
    }

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

      MDNS_RESULT result = read();
      if(result != E_MDNS_OK) {
        return INADDR_NONE;
      }
    }

    return INADDR_NONE;
  }

  void Resolver::query(const char* name) {
    Query query(name);
    udp.beginPacketMulticast(MDNS_BROADCAST_IP, MDNS_PORT, localIP, UDP_TIMEOUT);
    query.sendPacket(udp);
    udp.endPacket();
  }

  void Resolver::loop() {
    // Clear the buffers out.
    // This wouldn't be needed if the UDP library had the ability to
    // leave a multicast group
    unsigned int len = udp.parsePacket();
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * len);
    if(buffer != NULL) {
      udp.read(buffer, len);
      free(buffer);
    }
  }

  MDNS_RESULT Resolver::read() {
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
        // Silently bombing out, possibly isn't great, but it'll do for the moment.
        return E_MDNS_OUT_OF_MEMORY;
      }

      udp.read(buffer, len);
      lastResult = Answer::process(buffer, len, cache);

      free(buffer);
      return lastResult;
    }

    return E_MDNS_OK;
  }

  bool Resolver::isMDNSName(const char* name) {
    int len = strlen(name);
    int tldLen = strlen(MDNS_TLD);

    if(len < tldLen) {
      return false;
    }

    return strcmp(name + len - tldLen, MDNS_TLD) == 0;
  }
};
