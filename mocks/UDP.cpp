#include "UDP.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

UDP::UDP() {
  this->readBuffer = NULL;
  this->writeBuffer = NULL;
  this->readBufferLength = 0;
  this->writeBufferLength = 0;
};

UDP::UDP(unsigned char *buffer, int length) {
  this->readBuffer = NULL;
  this->writeBuffer = NULL;
  this->readBufferLength = 0;
  this->writeBufferLength = 0;
  this->setReadBuffer(buffer, length);
}

UDP::~UDP() {
  if(this->readBuffer != NULL) {
    free(this->readBuffer);
    this->readBuffer = NULL;
  }

  if(this->writeBuffer != NULL) {
    free(this->writeBuffer);
    this->writeBuffer = NULL;
  }
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
  if(this->readBuffer != NULL) {
    free(this->readBuffer);
    this->readBuffer = NULL;
  }

  this->readBuffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
  memcpy(this->readBuffer, buffer, length);
  this->readBufferLength = length;
}

uint8_t UDP::beginMulticast(IPAddress interfaceAddr, IPAddress multicast, uint16_t port) {
  return 0;
}

int UDP::beginPacket(IPAddress ip, uint16_t port) {
  return 0;
}

int UDP::beginPacket(const char *host, uint16_t port) {
  return 0;
}

int UDP::beginPacketMulticast(IPAddress multicastAddress, uint16_t port, IPAddress interfaceAddress, int ttl) {
  return 0;
}

int UDP::endPacket() {
  return 0;
}




int UDP::writeLength() {
  return this->writeBufferLength;
}

int UDP::readLength() {
  return this->readBufferLength;
}

int UDP::read() {
  return 0;
}

int UDP::read(unsigned char *buffer, size_t len) {
  memcpy(buffer, this->readBuffer, len);
  return len;
}

int UDP::peek() {
  return 0;
}

void UDP::flush() {}

size_t UDP::write(uint8_t) {
  return 0;
}

size_t UDP::write(const uint8_t *buffer, size_t size) {
  if(this->writeBuffer != NULL) {
    free(this->writeBuffer);
    this->writeBuffer = NULL;
  }

  this->writeBuffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
  memcpy(this->writeBuffer, buffer, size);
  this->writeBufferLength = size;

  return size;
}

int UDP::parsePacket() {
  return this->readBufferLength;
}
