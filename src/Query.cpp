#include "Query.h"

namespace mDNSResolver {
  Query::Query(std::string name) {
    this->assemblePacket(name);
  }

  Query::~Query() {}

  // If we pass in a UDP proxy, we can dynamically allocate the
  // memory without fee of fragmentation, and don't risk losing 
  // the reference if this object disappears
  unsigned char &Query::getPacket() {
    return *this->buffer;
  }

  unsigned int Query::length() {
    return this->packetLength;
  }

  void Query::assemblePacket(std::string &name) {
    for(int i = 0; i < MDNS_MAX_NAME_LEN + 18; i++) {
      this->buffer[i] = '\0';
    }

    this->buffer[4] = 0;
    this->buffer[5] = 1;

    int bufferIndex = 12;

    bufferIndex = buildDNSName(name, bufferIndex);
    
    int qtype = 0x01;
    int qclass = 0x01;
    this->buffer[bufferIndex++] = '\0';
    this->buffer[bufferIndex++] = (qtype & 0xFF00) >> 8;
    this->buffer[bufferIndex++] = qtype & 0xFF;
    this->buffer[bufferIndex++] = (qclass & 0xFF00) >> 8;
    this->buffer[bufferIndex++] = qclass & 0xFF;

    this->packetLength = bufferIndex;
  }

  int Query::buildDNSName(std::string &name, unsigned int bufferIndex) {
    int wordstart = 0, wordend = 0;
    
    do {
      if(name[wordend] == '.' || name[wordend] == '\0') {
        const int wordlen = wordend - wordstart;
        buffer[bufferIndex++] = (unsigned char)wordlen;

        for(int i = wordstart; i < wordend; i++) {
          buffer[bufferIndex++] = name[i];
        }
        
        if(name[wordend] == '.') {
          wordend++;
        }
        wordstart = wordend;
      }
      
      wordend++;
    } while(name[wordstart] != '\0');
    
    return bufferIndex;
  }
};