#ifndef MDNS_ANSWER_h
#define MDNS_ANSWER_h

#include "Constants.h"
#include <string>
#include <array>
#include <vector>

namespace mDNSResolver {
  class Answer {
  public:
    Answer();
    ~Answer();
    unsigned char* name;
    unsigned char* data;

    static MDNS_RESULT parse(unsigned char* buffer, unsigned int len);

#ifndef TEST
  private:
#endif
    static MDNS_RESULT skipQuestions(unsigned char* buffer, unsigned int len, unsigned int* offset);

  private:
    unsigned int type;
    unsigned int aclass;
    unsigned int cacheflush;
    unsigned long ttl;
    unsigned int len;
  };
};

#endif
