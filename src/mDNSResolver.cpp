#include "mDNSResolver.h"

namespace mDNSResolver {
  Cache cache;
  UDP udp;

  Resolver::Resolver() {}

  Resolver::~Resolver() {}

  IPAddress Resolver::search(std::string name) {
    Query query(name);
    int index = cache.search(name);

    while(!cache[index].resolved) {
      udp.beginPacketMulticast(IPAddress(224, 0, 0, 251), 5353, WiFi.localIP(), 255);
      query.sendPacket(udp);
      udp.endPacket();
      loop();
    }

    return cache[index].ipAddress;
  }

  void Resolver::loop() {
    cache.expire();

    unsigned int len = Udp.parsePacket();
    if(len > 0) {
      if(!init) {
        init = true;
        udp.beginMulticast(WiFi.localIP(), IPAddress(224, 0, 0, 251), 5353);
      }

      byte *buffer = (byte *)malloc(sizeof(byte) * len);
      Udp.read(buffer, len);
      Answer.process(buffer, len, &cache);
      free(buffer);
    }
  }
};
