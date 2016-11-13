#ifndef MDNS_ANSWER_h
#define MDNS_ANSWER_h

#include "Constants.h"
#include "Response.h"
#include "Cache.h"

#include <string>
#include <array>
#include <vector>

namespace mDNSResolver {
  class Answer {
  public:
    static MDNS_RESULT resolve(unsigned char *buffer, unsigned int len, unsigned int *offset, Cache& cache);

#ifndef TEST
  private:
#endif
    static int assembleName(unsigned char *buffer, unsigned int len, unsigned int *offset, char **name, unsigned int maxlen);
    static int assembleName(unsigned char *buffer, unsigned int len, unsigned int *offset, char **name);
    static MDNS_RESULT skipQuestions(unsigned char* buffer, unsigned int len, unsigned int* offset);
    static MDNS_RESULT parseName(char** name, const char* mapped, unsigned int len);
  };
};

#endif
