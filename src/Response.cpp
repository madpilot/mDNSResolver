#include "Response.h"

namespace mDNSResolver {
  Response::Response() {}

  Response::Response(std::string name) {
    this->name = name;
  }

  Response::Response(std::string name, unsigned long ttl) {
    this->name = name;
    this->ttl = ttl;
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
