#ifndef IPADDRESS_H
#define IPADDRESS_H
#include <string>

class IPAddress {
public:
  IPAddress();
  IPAddress(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
  bool operator==(const IPAddress& ipaddr);
  std::string toString();

private:
  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
};
#endif
