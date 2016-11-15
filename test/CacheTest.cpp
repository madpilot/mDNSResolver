#include "catch.h"
#include "Cache.h"
#include "Response.h"

using namespace mDNSResolver;
SCENARIO("DNS responses are stored in a Cache") {
  Cache cache;

  GIVEN("An empty cache") {
    THEN("the size of the cache should be 0") {
      REQUIRE(cache.length() == 0);
    }

    WHEN("a response is inserted") {
      Response r("response.local");
      cache.insert(r);

      THEN("the size of the cache should be 1") {
        REQUIRE(cache.length() == 1);
      }

      THEN("the first element of the cache should be the inserted response") {
        REQUIRE(cache[0] == r);
      }
    }
  }

  GIVEN("A non-empty cache") {
    Response r1("response1.local", 10);
    Response r2("response2.local", 5);
    cache.insert(r1);
    cache.insert(r2);

    THEN("the size of the cache should equal the number of items in the cache") {
      REQUIRE(cache.length() == 2);
    }

    WHEN("inserting another item") {
      Response r3("response3.local", 1);
      cache.insert(r3);

      THEN("the size of the cache should increase by one") {
        REQUIRE(cache.length() == 3);
      }

      THEN("the inserted item should appear in the cache") {
        REQUIRE(cache.search(r3) != -1);
      }
    }

    WHEN("inserting another item with the same name") {
      Response r2a("response2.local", 7);
      cache.insert(r2a);

      THEN("the existing item should be replaced") {
        REQUIRE(cache.length() == 2);
        REQUIRE(cache[1].ttl == 7);
      }
    }

    WHEN("an item is removed") {
      cache.remove(r2);

      THEN("the length should decrease") {
        REQUIRE(cache.length() == 1);
      }

      THEN("removes references to the item") {
        REQUIRE(cache.search(r1) != -1);
        REQUIRE(cache.search(r2) == -1);
      }
    }

    GIVEN("an item not in the cache") {
      WHEN("attempting to remove") {
        Response r3("response3.local", 1);
        cache.remove(r3);

        THEN("no items are removed") {
          REQUIRE(cache.search(r1) != -1);
          REQUIRE(cache.search(r2) != -1);
        }
      }
    }

    GIVEN("an item that references another item") {
      Response r3("response3.local", 1);
      r3.cname = &cache[cache.search(r2)];
      cache.insert(r3);

      WHEN("Removing the referenced item") {
        cache.remove(r2);

        THEN("The reference will be NULL") {
          REQUIRE(cache[cache.search(r3)].cname == NULL);
        }
      }
    }
  }

  GIVEN("A full cache") {
    Response r1("response1.local", 10);
    Response r2("response2.local", 5);
    Response r3("response3.local", 1);
    Response r4("response4.local", 8);

    cache.insert(r1);
    cache.insert(r2);
    cache.insert(r3);
    cache.insert(r4);

    // TODO: There is probably some macro magic that can do this based
    // on the MDNS_RESOLVER_MAX_CACHE value
    THEN("the size of the cache should be MDNS_RESOLVER_MAX_CACHE") {
      REQUIRE(cache.length() == MDNS_RESOLVER_MAX_CACHE);
    }

    THEN("each response should be in the cache") {
      REQUIRE(cache.search(r1) != -1);
      REQUIRE(cache.search(r2) != -1);
      REQUIRE(cache.search(r3) != -1);
      REQUIRE(cache.search(r4) != -1);
    }

    WHEN("a new item is inserted") {
      Response r5("response5.local", 10);
      cache.insert(r5);

      THEN("the length should not increase") {
        REQUIRE(cache.length() == MDNS_RESOLVER_MAX_CACHE);
      }

      THEN("the oldest response with the shortest time to live should be removed") {
        REQUIRE(cache.search(r1) != -1);
        REQUIRE(cache.search(r2) != -1);
        REQUIRE(cache.search(r3) == -1);
        REQUIRE(cache.search(r4) != -1);
        REQUIRE(cache.search(r5) != -1);
      }
    }

    WHEN("inserting another item with the same name") {
      Response r2a("response2.local", 7);
      cache.insert(r2a);

      THEN("the existing item should be replaced") {
        REQUIRE(cache[1].ttl == 7);
      }

      THEN("The oldest item should not be removed") {
        REQUIRE(cache.search(r3) != -1);
      }
    }
  }
}

SCENARIO("Expiring items in the cache") {
  Cache cache;

  GIVEN("A non empty cache") {
    Response r1("response1.local", 10);
    Response r2("response2.local", 2);
    cache.insert(r1);
    cache.insert(r2);

    WHEN("expire is called all of the response ttls should be reduced") {
      cache[0].ttl = 10;
      cache[1].ttl = 2;

      cache.expire();
      REQUIRE(cache[0].ttl == 9);
      REQUIRE(cache[1].ttl == 1);
    }

    GIVEN("an item with a TTL of 1") {
      Response r3("response3.local", 1);
      cache.insert(r3);

      cache[0].ttl = 10;
      cache[1].ttl = 2;

      WHEN("expire is called the item should be removed") {
        cache.expire();
        REQUIRE(cache.length() == 2);
        REQUIRE(cache.search(r3) == -1);
      }
    }
  }
}
