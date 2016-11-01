#ifndef MDNS_ANSWER_h
#define MDNS_ANSWER_h

#include "Constants.h"
#include "Response.h"
#include <string>
#include <array>
#include <vector>

namespace mDNSResolver {
  class Answer {
  public:
    Answer();
    ~Answer();
    static MDNS_RESULT buildResponse(unsigned char* buffer, unsigned int len, Response& response);

    char *name;
    unsigned int type;
    unsigned int aclass;
    unsigned int cacheflush;
    unsigned long ttl;
    unsigned int len;
    unsigned char *data;

#ifndef TEST
  private:
#endif
    static MDNS_RESULT parseAnswer(unsigned char* buffer, unsigned int len, unsigned int *offset, Answer* answer);
    static int assembleName(unsigned char *buffer, unsigned int len, unsigned int *offset, char **name, unsigned int maxlen);
    static int assembleName(unsigned char *buffer, unsigned int len, unsigned int *offset, char **name);
    static MDNS_RESULT skipQuestions(unsigned char* buffer, unsigned int len, unsigned int* offset);
    static MDNS_RESULT parseName(char** name, const char* mapped, unsigned int len);
  };
};

#endif
