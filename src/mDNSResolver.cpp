#include "mDNSResolver.h"
#include "Arduino.h"
#include <assert.h>

namespace mDNSResolver {
  Cache cache;

  Resolver::Resolver(WiFiUDP& udp) {
    timeout = 0;
    found = false;
    this->udp = udp;
    this->localIP = IPAddress(127, 0, 0, 1);
  }

  Resolver::Resolver(WiFiUDP& udp, IPAddress localIP) {
    timeout = 0;
    found = false;
    this->udp = udp;
    this->localIP = localIP;
  }

  Resolver::~Resolver() {}

  void Resolver::setLocalIP(IPAddress localIP) {
    this->localIP = localIP;
  }

  const char* Resolver::resolve(const char* name) {
    if(search(name)) {
      printf("Found\n");
      Serial.println(lastIPAddress);
      return lastIPAddress.toString().c_str();
    } else {
      printf("Not found\n");
      return name;
    }
  }

  bool Resolver::search(std::string name) {
    cache.expire();

    int attempts = 0;
    found = false;

    int index = cache.search(name);
    if(index == -1) {
      cache.insert(Response(name, 5));
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
      return IPAddress(0, 0, 0, 0);
    }
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
      udp.read(buffer, len);
      Answer::process(buffer, len, cache);
      free(buffer);
    }
  }
};
