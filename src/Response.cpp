#include "Response.h"
#ifdef TEST
#include <string.h>
#endif

namespace mDNSResolver {
  Response::Response() {
    this->name[0] = '\0';
    this->resolved = false;
    this->ttl = 0;
    this->cname = NULL;
    this->ipAddress = INADDR_NONE;
  }

  Response::Response(const char* name) {
    strncpy(this->name, name, MDNS_MAX_NAME_LEN + 1);
    this->name[MDNS_MAX_NAME_LEN + 1] = '\0';
    this->ttl = 0;
    this->resolved = false;
    this->cname = NULL;
    this->ipAddress = INADDR_NONE;
  }

  Response::Response(const char* name, unsigned long ttl) {
    strncpy(this->name, name, MDNS_MAX_NAME_LEN + 1);
    this->name[MDNS_MAX_NAME_LEN + 1] = '\0';
    this->ttl = ttl;
    this->resolved = false;
    this->cname = NULL;
    this->ipAddress = INADDR_NONE;
  }

  Response::~Response() {
  }

  void Response::decrementTTL() {
    if(this->ttl > 0) {
      this->ttl--;
    }
  }

  bool Response::operator==(const Response& response) {
    return strcmp(this->name, response.name) == 0;
  }

  bool Response::operator!=(const Response& response) {
    return strcmp(this->name, response.name) != 0;
  }
}
