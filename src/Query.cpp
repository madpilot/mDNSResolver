#include "Query.h"

namespace mDNSResolver {
  Query::Query(const char* name) {
    this->name = name;
  }

  Query::~Query() {}

  // If we pass in a UDP proxy, we can dynamically allocate the
  // memory without fee of fragmentation, and don't risk losing
  // the reference if this object disappears
  void Query::sendPacket(WiFiUDP& socket) {
    int bufferLength = strlen(this->name) + 18;
    unsigned char buffer[bufferLength];
    assemblePacket(buffer, bufferLength);
    socket.write(buffer, bufferLength);
  }

  void Query::assemblePacket(unsigned char *buffer, int bufferLength) {
    for(int i = 0; i < bufferLength; i++) {
      buffer[i] = 0;
    }

    buffer[5] = 0x01;

    int bufferIndex = 12;
    bufferIndex = buildDNSName(buffer, bufferIndex);

    int qtype = 0x01;
    int qclass = 0x01;

    buffer[bufferIndex++] = '\0';
    buffer[bufferIndex++] = (qtype & 0xFF00) >> 8;
    buffer[bufferIndex++] = qtype & 0xFF;
    buffer[bufferIndex++] = (qclass & 0xFF00) >> 8;
    buffer[bufferIndex++] = qclass & 0xFF;
  }

  int Query::buildDNSName(unsigned char *buffer, unsigned int bufferIndex) {
    int wordstart = 0, wordend = 0;

    do {
      if(this->name[wordend] == '.' || this->name[wordend] == '\0') {
        const int wordlen = wordend - wordstart;
        buffer[bufferIndex++] = (unsigned char)wordlen;

        for(int i = wordstart; i < wordend; i++) {
          buffer[bufferIndex++] = this->name[i];
        }

        if(this->name[wordend] == '.') {
          wordend++;
        }
        wordstart = wordend;
      }

      wordend++;
    } while(this->name[wordstart] != '\0');

    return bufferIndex;
  }
};
