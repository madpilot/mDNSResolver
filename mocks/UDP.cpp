#include "UDP.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

UDP::UDP() {
  this->readBuffer = 0;
  this->writeBuffer = 0;
  this->readBufferLength = 0;
  this->writeBufferLength = 0;
};

UDP::UDP(unsigned char *buffer, int length) {
  this->setReadBuffer(buffer, length);
}

UDP::~UDP() {
  if(this->readBuffer) {
    free(this->readBuffer);
  }

  if(this->writeBuffer) {
    free(this->writeBuffer);
  }
}

void begin(IPAddress dest, int port) {

}

void beginMulticast(IPAddress src, IPAddress dest, int port) {

}

void beginMulticast(IPAddress src, IPAddress dest, int port, int timeout) {

}

void endPacket() {

}

UDP UDP::loadFromFile(const char *path) {
  unsigned char *source = NULL;
  UDP udp;

  FILE *fp = fopen(path, "rb");

  if(fp != NULL) {
    if(fseek(fp, 0L, SEEK_END) != 0) {
      fprintf(stderr, "ERROR: Unable to seek to the end of the file: %s\n", path);
      fclose(fp);
      return udp;
    }

    long bufsize = ftell(fp);
    if(bufsize == -1) {
      fprintf(stderr, "ERROR: Unable to get file position\n");
      fclose(fp);
      return udp;
    }

    source = (unsigned char *)malloc(sizeof(unsigned char) * (bufsize + 1));
    if(fseek(fp, 0L, SEEK_SET) != 0) {
      fprintf(stderr, "ERROR: Unable to see to the beginning of the file: %s\n", path);
      fclose(fp);
      return udp;
    }

    size_t len = fread(source, sizeof(unsigned char), bufsize, fp);
    if(ferror(fp) != 0) {
      fprintf(stderr, "ERROR: Unable to read the file (%i): %s\n", ferror(fp), path);
      fclose(fp);
      return udp;
    }

    udp.setReadBuffer(source, len);
    fclose(fp);
    return udp;
  } else {
    fprintf(stderr, "ERROR: Unable to seek to open file: %s\n", path);
  }

  return udp;
}

void UDP::setReadBuffer(unsigned char *buffer, int length) {
  if(this->readBuffer) {
    free(this->readBuffer);
  }

  this->readBuffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
  memcpy(this->readBuffer, buffer, length);
  this->readBufferLength = length;
}

int UDP::writeLength() {
  return this->writeBufferLength;
}

int UDP::readLength() {
  return this->readBufferLength;
}

void UDP::read(unsigned char *buffer, int length) {
  memcpy(buffer, this->readBuffer, length);
}

void UDP::write(unsigned char *buffer, int length) {
  if(this->writeBuffer) {
    free(this->writeBuffer);
  }

  this->writeBuffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
  memcpy(this->writeBuffer, buffer, length);
  this->writeBufferLength = length;
}

unsigned int UDP::parsePacket() {
  return this->readBufferLength;
}
