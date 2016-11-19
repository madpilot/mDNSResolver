#include "catch.h"

#include <string.h>
#include <stdlib.h>

#include "mDNSResolver.h"

#include <WiFiUdp.h>
#include <IPAddress.h>

using namespace mDNSResolver;

SCENARIO("resolving an mDNS name") {
  IPAddress localIP(192, 168, 0, 1);
  WiFiUDP* Udp = WiFiUDP::loadFromFile("fixtures/cname_answer.bin");

  WHEN("searching for a name") {
    Resolver resolver(*Udp, localIP);

    GIVEN("a search") {
      resolver.search("test.local");

      THEN("a UDP request should be made") {
        unsigned char expected[] = {
          0x00, // Unused
          0x00, // Unused
          0x00, // Unused
          0x00, // Unused
          0x00, // Unused
          0x01, // Query
          0x00, // Unused
          0x00, // Unused
          0x00, // Unused
          0x00, // Unused
          0x00, // Unused
          0x00, // Unused
          0x04, // Length of test
          't', 'e', 's', 't',
          0x05, // Length of local
          'l', 'o', 'c', 'a', 'l',
          0x00, // Null terminator
          0x00, // QType
          0x01, // QType
          0x00, // QClass
          0x01, // QClass
        };

        for(int i = 0; i < Udp->writeLength(); i++) {
          REQUIRE(Udp->writeBuffer[i] == expected[i]);
        }
      }
    }

    GIVEN("a name that does not exist") {
      IPAddress result = resolver.search("test.local");

      THEN("search should return false") {
        REQUIRE(result == INADDR_NONE);
      }
    }

    GIVEN("a name that does exist") {
      IPAddress result = resolver.search("mqtt.local");

      THEN("search should return true") {
        REQUIRE(result == IPAddress(192, 168, 1, 2));
      }
    }
  }

  delete Udp;
}
