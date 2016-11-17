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

SCENARIO("Equality") {
  GIVEN("A response") {
    Response r1("test.local");

    WHEN("Testing against a response with the same name") {
      Response r2("test.local");

      THEN("they two objects should be equal") {
        REQUIRE(r1 == r2);
      }
    }

    WHEN("Testing against a response with a different name") {
      Response r2("test2.local");

      THEN("they two objects should not be equal") {
        REQUIRE(r1 != r2);
      }
    }

  }
}
