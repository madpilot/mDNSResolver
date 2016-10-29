#include "catch.h"
#include "Response.h"

using namespace mDNSResolver;
SCENARIO("Decrementing the TTL") {
  Response r;

  GIVEN("a non-zero ttl") {
    r.ttl = 10;

    WHEN("decrementing") {
      r.decrementTTL();

      THEN("the ttl should reduce by 1") {
        REQUIRE(r.ttl == 9);
      }
    }
  }

  GIVEN("a zero ttl") {
    r.ttl = 0;

    WHEN("decrementing") {
      r.decrementTTL();

      THEN("the ttl should stay at 0") {
        REQUIRE(r.ttl == 0);
      }
    }
  }
}
