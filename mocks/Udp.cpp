#include "Udp.h"
#include <stdlib.h>
#include <string.h>

UDP::UDP() {
  this->buffer = 0;
  this->bufferLength = 0;
};

UDP::~UDP() {
  if(this->buffer) {
    free(this->buffer);
  }
}

int UDP::length() {
  return this->bufferLength;
}

void UDP::write(unsigned char *buffer, int length) {
  this->buffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
  memcpy(this->buffer, buffer, length);
  this->bufferLength = length;
}