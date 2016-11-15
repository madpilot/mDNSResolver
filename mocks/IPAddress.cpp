#include "IPAddress.h"

IPAddress::IPAddress() {};
IPAddress::IPAddress(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
}


bool IPAddress::operator==(const IPAddress& ipaddr) {
  return this->a == ipaddr.a && this->b == ipaddr.b && this->c == ipaddr.c && this->d == ipaddr.d;
}

std::string IPAddress::toString() {
  std::string buffer = std::to_string(this->a) + "." + std::to_string(this->b) + "." + std::to_string(this->c) + "." + std::to_string(this->d);
  return buffer;
}
