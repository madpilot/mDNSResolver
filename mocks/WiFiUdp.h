#ifndef WIFI_UDP_H
#define WIFI_UDP_H

#include "IPAddress.h"

class WiFiUDP {
public:
  WiFiUDP();
  // Initialise the read buffer
  WiFiUDP(unsigned char *packet, int length);
  ~WiFiUDP();

  static WiFiUDP* loadFromFile(const char *path);

  void setReadBuffer(unsigned char *buffer, int length);

  uint8_t begin(uint16_t port);
  void stop();
  uint8_t beginMulticast(IPAddress interfaceAddr, IPAddress multicast, uint16_t port);
  int beginPacket(IPAddress ip, uint16_t port);
  int beginPacket(const char *host, uint16_t port);
  int beginPacketMulticast(IPAddress multicastAddress, uint16_t port, IPAddress interfaceAddress, int ttl = 1);
  int endPacket();

  int readLength();
  int writeLength();

  int read();
  int read(unsigned char* buffer, size_t len);
  int read(char* buffer, size_t len) { return read((unsigned char*)buffer, len); };
  int peek();
  void flush();

  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  int parsePacket();

  unsigned char *readBuffer;
  unsigned char *writeBuffer;
  int readBufferLength;
  int writeBufferLength;
};

#endif
