#include "Response.h"

namespace mDNSResolver {
  Response::Response() {
    this->resolved = false;
    this->ttl = 0;
  }

  Response::Response(std::string name) {
    this->name = name;
    this->ttl = 0;
    this->resolved = false;
  }

  Response::Response(std::string name, unsigned long ttl) {
    this->name = name;
    this->ttl = ttl;
    this->resolved = false;
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
