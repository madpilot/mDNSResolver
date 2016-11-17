#include "Response.h"

namespace mDNSResolver {
  Response::Response() {
    this->resolved = false;
    this->ttl = 0;
    this->cname = NULL;
    this->ipAddress = INADDR_NONE;
  }

  Response::Response(std::string name) {
    this->name = name;
    this->ttl = 0;
    this->resolved = false;
    this->cname = NULL;
    this->ipAddress = INADDR_NONE;
  }

  Response::Response(std::string name, unsigned long ttl) {
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
    return this->name == response.name;
  }
}
