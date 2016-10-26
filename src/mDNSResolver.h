#ifndef MDNS_RESOLVER_h
#define MDNS_RESOLVER_h

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define mdns_result                       uint8_t
#define E_MDNS_OK                         0x20
#define E_MDNS_TOO_BIG                    0x21
#define E_MDNS_POINTER_OVERFLOW           0x22
#define E_MDNS_PACKET_ERROR               0x23
#define E_MDNS_PARSING_ERROR              0x24

#define MDNS_TARGET_PORT 5353
#define MDNS_SOURCE_PORT 5353
#define MDNS_TTL 255
#define MDNS_RESOLVER_MAX_CACHE 4
#define MDNS_TIMEOUT 5
#define MDNS_MAX_NAME_LEN 255
// Max allowable packet - resolving names should result in small packets.
#define MDNS_MAX_PACKET 256

namespace mDNSResolver {
  typedef struct Query {
    char *name;
    unsigned int type;
    unsigned int qclass;
    unsigned int unicastResponse;
    bool valid;
  };

  typedef struct Answer {
    char *name;
    unsigned int type;
    unsigned int aclass;
    unsigned int cacheflush;
    unsigned long ttl;
    unsigned int len;
    byte *data;
  };
  
  typedef struct Response {
    char *name;
    IPAddress ipAddress;
    unsigned long int ttl;
    bool waiting;
    unsigned long int timeout;
  };
  
  class Resolver {
    public:
      Resolver();
      ~Resolver();
      mdns_result loop();
      Response query(const char *name);
      int search(const char *name);
    private:
      int _cacheCount;
      bool _init;
      Response _cache[MDNS_RESOLVER_MAX_CACHE];
      unsigned long _lastSweep;

      void init();
      mdns_result parseName(char **name, const char *mapped, unsigned int mappedlen);
      mdns_result assembleName(byte *buffer, unsigned int len, unsigned int *offset, char **name, unsigned int maxlen);
      mdns_result assembleName(byte *buffer, unsigned int len, unsigned int *offset, char **name);

      mdns_result skipQuestions(byte *buffer, unsigned int len, unsigned int *offset_ptr);
      mdns_result parseAnswer(byte *buffer, unsigned int len, unsigned int *offset_ptr, Answer *a_ptr);
      mdns_result parsePacket(byte *buffer, unsigned int len);
      void expire();
      
      Response buildResponse(const char *name);
      void freeResponse(Response r);
      
      Query buildQuery(const char *name);
      void freeQuery(Query q);
      void broadcastQuery(Query q);

      mdns_result listen();
      
      void insert(Response response);
      void remove(int index);
  };
};
#endif
