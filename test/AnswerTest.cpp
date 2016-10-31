#include "catch.h"
#include "Cache.h"

#include "Constants.h"
#include "Answer.h"

#include <UDP.h>

using namespace mDNSResolver;
SCENARIO("mDNS packet with a question is received.") {
  GIVEN("the packet has questions") {
    UDP Udp = UDP::loadFromFile("fixtures/question.bin");
    unsigned len = Udp.parsePacket();
    unsigned char *packet = (unsigned char *)malloc(sizeof(unsigned char) * len);

    Udp.read(packet, len);

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
  }

  GIVEN("the packet is malicious and attempts to overflow the pointer") {
    UDP Udp = UDP::loadFromFile("fixtures/overflow_question.bin");
    unsigned len = Udp.parsePacket();
    unsigned char *packet = (unsigned char *)malloc(sizeof(unsigned char) * len);

    Udp.read(packet, len);

    WHEN("parsing") {
      unsigned int offset = 0;
      int result = Answer::skipQuestions(packet, len, &offset);

      THEN("an error should be returned") {
        REQUIRE(result == E_MDNS_PACKET_ERROR);
      }
    }
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
        REQUIRE(r == E_MDNS_INVALID_LABEL_LENGTH);
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
        REQUIRE(r == E_MDNS_INVALID_LABEL_LENGTH);
      }

      free(result);
    }
  }
}

SCENARIO("assembling a name") {
  GIVEN("a full name") {
    unsigned char buffer[] = {
      0x04, 't', 'e', 's', 't',
      0x07, 'l', 'o', 'c', 'a', 'l', 0x00,
      0x05, 't', 'e', 's', 't', '2',
      0xc0, 0x05, // Points at local
      0xc0, 0x00, // Points at test.local
      0xc0, 0x0c, // Points at test2.local
    };

    WHEN("assembling") {
      char* result = (char *)malloc(sizeof(char) * MDNS_MAX_NAME_LEN);
      int len = Answer::assembleName(buffer, 24, 0, &result);

      THEN("the returned value should be the same as the source length") {
        REQUIRE(len == 12);
      }

      THEN("the name returns the name unchanged") {
        REQUIRE(memcmp(result, buffer, 12) == 0);
      }

      free(result);
    }
  }
}
