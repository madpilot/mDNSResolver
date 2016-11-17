#include "Response.h"

namespace mDNSResolver {
  Response::Response() {
    this->name = "";
    this->resolved = false;
    this->ttl = 0;
    this->cname = NULL;
    this->ipAddress = INADDR_NONE;
  }

  Response::Response(const char* name) {
    this->name = name;
    this->ttl = 0;
    this->resolved = false;
    this->cname = NULL;
    this->ipAddress = INADDR_NONE;
  }

  Response::Response(const char* name, unsigned long ttl) {
    this->name = name;
    this->ttl = ttl;
    this->resolved = false;
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
