#include "catch.h"
#include "Cache.h"

#include "Constants.h"
#include "Answer.h"

#include <UDP.h>

using namespace mDNSResolver;
SCENARIO("UDP packet is received.") {
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

  GIVEN("the packet is malicious and attempting to overflow the pointer") {
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

