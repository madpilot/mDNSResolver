#include "catch.h"

#include <string.h>
#include <stdlib.h>

#include "mDNSResolver.h"

#include <UDP.h>
#include <IPAddress.h>

using namespace mDNSResolver;

SCENARIO("resolving an mDNS name") {
  IPAddress localIP(192, 168, 0, 1);
  UDP Udp = UDP::loadFromFile("fixtures/cname_answer.bin");
  Resolver resolver(Udp, localIP);

  WHEN("searching for a name") {
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

        for(int i = 0; i < Udp.writeLength(); i++) {
          REQUIRE(Udp.writeBuffer[i] == expected[i]);
        }
      }
    }

    GIVEN("a name that does not exist") {
      bool result = resolver.search("test.local");

      THEN("search should return false") {
        REQUIRE(result == false);
      }
    }

    GIVEN("a name that does exist") {
      bool result = resolver.search("mqtt.local");

      THEN("search should return false") {
        REQUIRE(result == true);
      }

      THEN("the IP Address should be set") {
        REQUIRE(resolver.address() == IPAddress(192, 168, 1, 2));
      }
    }
  }
}
