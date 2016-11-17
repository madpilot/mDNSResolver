#include "catch.h"
#include "Cache.h"

#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Answer.h"
#include "Cache.h"

#include <WiFiUdp.h>

using namespace mDNSResolver;

SCENARIO("resolving a packet") {
  WiFiUDP* Udp = WiFiUDP::loadFromFile("fixtures/cname_answer.bin");
  unsigned len = Udp->parsePacket();
  unsigned char *packet = (unsigned char *)malloc(sizeof(unsigned char) * len);
  Udp->read(packet, len);

  GIVEN("an empty cache") {
    Cache cache;

    WHEN("parsing an A-record") {
      unsigned int offset = 68;
      Answer::resolve(packet, len, &offset, cache);

      THEN("the cache should still be empty") {
        REQUIRE(cache.length() == 0);
      }
    }

    WHEN("parsing an CNAME-record") {
      unsigned int offset = 12;
      Answer::resolve(packet, len, &offset, cache);

      THEN("the cache should still be empty") {
        REQUIRE(cache.length() == 0);
      }
    }
  }

  GIVEN("a cache") {
    Cache cache;

    GIVEN("an unresolved Response object") {
      Response response("test.local");
      cache.insert(response);

      WHEN("parsing an A-record answer with a name that does not match the Response object") {
        unsigned int offset = 68;
        Answer::resolve(packet, len, &offset, cache);

        THEN("the cache will not expand") {
          REQUIRE(cache.length() == 1);
        }

        THEN("the response object will still be unresolved") {
          REQUIRE(response.resolved == false);
        }
      }

      WHEN("parsing a CNAME-record answer with a name that does not match the Response object") {
        unsigned int offset = 12;
        Answer::resolve(packet, len, &offset, cache);

        THEN("the cache will not expand") {
          REQUIRE(cache.length() == 1);
        }

        THEN("the response object will still be unresolved") {
          REQUIRE(response.resolved == false);
        }
      }
    }

    GIVEN("an unresolved Response object") {
      Response response("nas.local");
      cache.insert(response);

      WHEN("parsing an A-record answer with a name that matches the Response object") {
        unsigned int offset = 68;
        MDNS_RESULT result = Answer::resolve(packet, len, &offset, cache);

        THEN("result should be ok") {
          REQUIRE(result == E_MDNS_OK);
        }

        THEN("the cache will not expand") {
          REQUIRE(cache.length() == 1);
        }

        THEN("the response request object will be resolved") {
          REQUIRE(cache[0].resolved == true);
        }

        THEN("the TTL will be set") {
          REQUIRE(cache[0].ttl == 120);
        }

        THEN("the response request object will have the correct IP address object") {
          REQUIRE(cache[0].ipAddress == IPAddress(192, 168, 1, 2));
        }
      }
    }

    GIVEN("an unresolved Response object") {
      Response response("mqtt.local");
      cache.insert(response);

      WHEN("parsing a complete CNAME packet") {
        MDNS_RESULT result = Answer::process(packet, len, cache);

        THEN("result should be ok") {
          REQUIRE(result == E_MDNS_OK);
        }

        THEN("the cache should have a new item in it") {
          REQUIRE(cache.length() == 2);
        }

        THEN("the resolved A-record will be in the cache") {
          REQUIRE(strcmp(cache[1].name, "nas.local") == 0);
        }

        THEN("the A-record will be resolved") {
          REQUIRE(cache[1].resolved == true);
        }

        THEN("the A-record will have the correct IP address") {
          REQUIRE(cache[1].ipAddress == IPAddress(192, 168, 1, 2));
        }

        THEN("the A-record will have the correct TTL") {
          REQUIRE(cache[1].ttl == 120);
        }

        THEN("the CNAME-record will be resolved") {
          REQUIRE(cache[0].resolved == true);
        }

        THEN("the CNAME-record will have the correct IP address") {
          REQUIRE(cache[0].ipAddress == IPAddress(192, 168, 1, 2));
        }

        THEN("the CNAME-record will have the correct TTL") {
          REQUIRE(cache[0].ttl == 60);
        }
      }

      WHEN("parsing an CNAME-record answer with a name that matches the Response object") {
        unsigned int offset = 12;

        WHEN("the pointed name is not in the cache") {
          MDNS_RESULT result = Answer::resolve(packet, len, &offset, cache);

          THEN("result should be ok") {
            REQUIRE(result == E_MDNS_OK);
          }

          THEN("the cache will expand") {
            REQUIRE(cache.length() == 2);
          }

          THEN("the Response object will point to the new Response object") {
            REQUIRE(*cache[0].cname == cache[1]);
          }

          THEN("the Response object cname pointer's name will match the data payload") {
            REQUIRE(strcmp(cache[0].cname->name, "nas.local") == 0);
          }
        }

        WHEN("the pointed name is in the cache") {
          Response response2("nas.local");
          cache.insert(response2);

          WHEN("the pointed object is not resolved") {
            MDNS_RESULT result = Answer::resolve(packet, len, &offset, cache);

            THEN("result should be ok") {
              REQUIRE(result == E_MDNS_OK);
            }

            THEN("the cache will not expand") {
              REQUIRE(cache.length() == 2);
            }

            THEN("the Response object will point to the old Response object") {
              REQUIRE(*cache[0].cname == cache[1]);
            }
          }

          WHEN("the pointed object is resolved") {
            cache[1].resolved = true;
            cache[1].ipAddress = IPAddress(192, 168, 1, 2);
            MDNS_RESULT result = Answer::resolve(packet, len, &offset, cache);

            THEN("result should be ok") {
              REQUIRE(result == E_MDNS_OK);
            }

            THEN("the cache will not expand") {
              REQUIRE(cache.length() == 2);
            }
          }

          cache.remove(response2);
        }
      }
    }
  }

  GIVEN("a full cache") {
    Cache cache;

    Response response1("test1.local", 255);
    Response response2("mqtt.local", 1);
    Response response3("test2.local", 255);
    Response response4("test3.local", 255);

    cache.insert(response1);
    cache.insert(response2);
    cache.insert(response3);
    cache.insert(response4);

    GIVEN("a packet with a CNAME that matches the lowest TTL item in the cache") {
      unsigned int offset = 12;

      WHEN("parsing") {
        MDNS_RESULT result = Answer::resolve(packet, len, &offset, cache);

        THEN("the CNAME entry should be removed") {
          REQUIRE(cache.search("mqtt.local") == -1);
        }

        THEN("the A record the CNAME pointed to should be entered") {
          REQUIRE(cache.search("nas.local") != -1);
        }
      }
    }
  }

  delete Udp;
}

SCENARIO("mDNS packet with a question is received.") {
  GIVEN("the packet has questions") {
    WiFiUDP* Udp = WiFiUDP::loadFromFile("fixtures/question.bin");
    unsigned len = Udp->parsePacket();
    unsigned char *packet = (unsigned char *)malloc(sizeof(unsigned char) * len);

    Udp->read(packet, len);

    WHEN("parsing") {
      unsigned int offset = 0;
      int result = Answer::skipQuestions(packet, len, &offset);

      THEN("questions are skipped") {
        REQUIRE(result == E_MDNS_OK);
      }

      THEN("offset has incremented correctly") {
        // We expect this to be one past the length of the file
        REQUIRE(offset == 45);
      }
    }

    delete Udp;
  }

  GIVEN("the packet is malicious and attempts to overflow the pointer") {
    WiFiUDP* Udp = WiFiUDP::loadFromFile("fixtures/overflow_question.bin");
    unsigned len = Udp->parsePacket();
    unsigned char *packet = (unsigned char *)malloc(sizeof(unsigned char) * len);

    Udp->read(packet, len);

    WHEN("parsing") {
      unsigned int offset = 0;
      int result = Answer::skipQuestions(packet, len, &offset);

      THEN("an error should be returned") {
        REQUIRE(result == E_MDNS_PACKET_ERROR);
      }
    }
    delete Udp;
  }
}

SCENARIO("Parsing a name") {
  GIVEN("a DNS encoded name") {
    char name[] = {
      0x04, 't', 'e', 's', 't',
      0x05, 'l', 'o', 'c', 'a', 'l'
    };

    WHEN("parsing") {
      char* result = (char *)malloc(sizeof(char) * 10);
      Answer::parseName(&result, name, 11);

      THEN("converts the name to a FQDN") {
        REQUIRE(strcmp(result, "test.local") == 0);
      }

      free(result);
    }
  }

  GIVEN("a long encoded name") {
    char name[] = {
      0x04, 't', 'h', 'i', 's',
      0x02, 'i', 's',
      0x01, 'a',
      0x04, 't', 'e', 's', 't',
      0x05, 'l', 'o', 'c', 'a', 'l'
    };

    WHEN("parsing") {
      char* result = (char *)malloc(sizeof(char) * 20);
      Answer::parseName(&result, name, 21);

      THEN("converts the name to a FQDN") {
        REQUIRE(strcmp(result, "this.is.a.test.local") == 0);
      }

      free(result);
    }
  }

  GIVEN("a DNS encoded name with an long label length") {
    char name[] = {
      0x3f, 't', 'e', 's', 't',
      0x05, 'l', 'o', 'c', 'a', 'l'
    };

    WHEN("parsing") {
      char* result = (char *)malloc(sizeof(char) * 10);
      int r = Answer::parseName(&result, name, 11);

      THEN("returns an E_MDNS_INVALID_LABEL_LENGTH") {
        REQUIRE(r == E_MDNS_PACKET_ERROR);
      }

      free(result);
    }
  }

  GIVEN("a DNS encoded name with an label length that is shorter than required length, but would cause a buffer overflow") {
    char name[] = {
      0x04, 't', 'e', 's', 't',
      0x07, 'l', 'o', 'c', 'a', 'l'
    };

    WHEN("parsing") {
      char* result = (char *)malloc(sizeof(char) * 10);
      int r = Answer::parseName(&result, name, 11);

      THEN("returns an E_MDNS_INVALID_LABEL_LENGTH") {
        REQUIRE(r == E_MDNS_PACKET_ERROR);
      }

      free(result);
    }
  }
}

SCENARIO("assembling a name") {
  GIVEN("a buffer") {
    unsigned char buffer[] = {
      0x04, 't', 'e', 's', 't',
      0x07, 'l', 'o', 'c', 'a', 'l', 0x00,
      0x05, 't', 'e', 's', 't', '2',
      0xc0, 0x05, // Points at local
      0xc0, 0x2d, // Overflow
      0xc0, 0x0c, // Points at test2.local
    };

    WHEN("assembling a full name") {
      char* result = (char *)malloc(sizeof(char) * MDNS_MAX_NAME_LEN);
      unsigned int offset = 0;
      int len = Answer::assembleName(buffer, 24, &offset, &result);

      THEN("the returned value should be the same as the source length") {
        REQUIRE(len == 12);
      }

      THEN("the name returns the name unchanged") {
        REQUIRE(memcmp(result, buffer, 12) == 0);
      }

      THEN("offset should be incremented") {
        REQUIRE(offset == 12);
      }

      free(result);
    }

    WHEN("assembling a name with a pointer") {
      char* result = (char *)malloc(sizeof(char) * MDNS_MAX_NAME_LEN);
      unsigned int offset = 12;
      int len = Answer::assembleName(buffer, 24, &offset, &result);

      THEN("the returned value should be the same as the source length") {
        REQUIRE(len == 13);
      }

      THEN("resolve the pointer") {
        unsigned char expected[] = {
          0x05, 't', 'e', 's', 't', '2',
          0x07, 'l', 'o', 'c', 'a', 'l', 0x00
        };

        REQUIRE(memcmp(result, expected, 13) == 0);
      }

      THEN("offset should be incremented") {
        REQUIRE(offset == 20);
      }

      free(result);
    }

    WHEN("assembling a name that is a pointer") {
      char* result = (char *)malloc(sizeof(char) * MDNS_MAX_NAME_LEN);
      unsigned int offset = 22;
      int len = Answer::assembleName(buffer, 24, &offset, &result);

      THEN("the returned value should be the same as the source length") {
        REQUIRE(len == 13);
      }

      THEN("resolve the pointer") {
        unsigned char expected[] = {
          0x05, 't', 'e', 's', 't', '2',
          0x07, 'l', 'o', 'c', 'a', 'l', 0x00
        };

        REQUIRE(memcmp(result, expected, 13) == 0);
      }

      THEN("offset should increment by 2") {
        REQUIRE(offset == 24);
      }

      free(result);
    }

    WHEN("assembling a name with an overflowing pointer") {
      char* result = (char *)malloc(sizeof(char) * MDNS_MAX_NAME_LEN);
      unsigned int offset = 20;
      int len = Answer::assembleName(buffer, 24, &offset, &result);

      THEN("the returned -1 * E_MDNS_POINTER_OVERFLOW") {
        REQUIRE(len == -1 * E_MDNS_POINTER_OVERFLOW);
      }

      free(result);
    }
  }
}
