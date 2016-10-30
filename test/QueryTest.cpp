#include "catch.h"
#include "Query.h"
#include <UDP.h>

TEST_CASE("Assemble Packet", "[Query]" ) {
  std::string name = std::string("test.local");
  mDNSResolver::Query q(name);
  UDP udp;

  SECTION("packet output") {
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

    q.sendPacket(&udp);

    for(int i = 0; i < udp.writeLength(); i++) {
      REQUIRE(udp.writeBuffer[i] == expected[i]);
    }
  }
}
