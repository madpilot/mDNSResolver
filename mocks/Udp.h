#ifndef UDP_H
#define UDP_H

class UDP {
public:
  UDP();
  // Initialise the read buffer
  UDP(unsigned char *packet, int length);
  ~UDP();

  static UDP loadFromFile(const char *path);

  void setReadBuffer(unsigned char *buffer, int length);

  int readLength();
  int writeLength();

  void read(unsigned char *buffer, int length);
  void write(unsigned char *buffer, int length);
  unsigned int parsePacket();

  unsigned char *readBuffer;
  unsigned char *writeBuffer;
  int readBufferLength;
  int writeBufferLength;
};

#endif
