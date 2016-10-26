#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "../src/Query.h"

TEST_CASE("Assemble Packet", "[Query]" ) {
  std::string name = std::string("test.local");
  mDNSResolver::Query q(name);

  SECTION("packet size") {
    REQUIRE(q.length() == 18 + name.length());
  }

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

    unsigned char actual[q.length()];
    memcpy(actual, &q.getPacket(), q.length());

    for(int i = 0; i < q.length(); i++) {
      REQUIRE(actual[i] == expected[i]);
    }
  }
}