#include "Answer.h"

#include <string.h>
#include <stdlib.h>

namespace mDNSResolver {
  Answer::Answer() {
    this->name = NULL;
    this->data = NULL;
  }

  Answer::~Answer() {
    if(this->name) {
      free(this->name);
    }
    if(this->data) {
      free(this->data);
    }
  }

  MDNS_RESULT Answer::resolve(unsigned char *buffer, unsigned int len, unsigned int* offset, Cache& cache) {
    char* assembled = (char *)malloc(sizeof(char) * MDNS_MAX_NAME_LEN);
    int nameLen = Answer::assembleName(buffer, len, offset, &assembled);

    if(nameLen == -1 * E_MDNS_POINTER_OVERFLOW) {
      free(assembled);
      return nameLen;
    }

    char *name = (char *)malloc(sizeof(char) * nameLen);
    parseName(&name, assembled, strlen(assembled));

    unsigned int type = (buffer[(*offset)++] << 8) + buffer[(*offset)++];
    unsigned int aclass = (buffer[(*offset)++] << 8) + buffer[(*offset)++];
    unsigned long ttl = (buffer[(*offset)++] << 24) + (buffer[(*offset)++] << 16) + (buffer[(*offset)++] << 8) + buffer[(*offset)++];
    unsigned int dataLen = (buffer[(*offset)++] << 8) + buffer[(*offset)++];

    int index = cache.search(name);
    if(type == MDNS_A_RECORD && index != -1) {
      if(dataLen == 4) {
        unsigned int a = (unsigned int)*(buffer + (*offset)++);
        unsigned int b = (unsigned int)*(buffer + (*offset)++);
        unsigned int c = (unsigned int)*(buffer + (*offset)++);
        unsigned int d = (unsigned int)*(buffer + (*offset)++);

        cache[index].resolved = true;
        cache[index].ttl = ttl;
        cache[index].ipAddress = IPAddress(a, b, c, d);

      } else {
        (*offset) += dataLen;
      }
    } else if(type == MDNS_CNAME_RECORD && index != -1) {
      unsigned int dataOffset = (*offset);
      (*offset) += dataLen;
      dataLen = Answer::assembleName(buffer, len, &dataOffset, &assembled, dataLen);

      if(dataLen == -1 * E_MDNS_POINTER_OVERFLOW) {
        free(assembled);
        return dataLen;
      }

      char *data = (char *)malloc(sizeof(char) * (dataLen - 1));
      // This will fragment...
      parseName(&data, assembled, dataLen - 1);

      std::string cname = std::string(data);
      int cnameIndex = cache.search(cname);

      Response *r;
      if(cnameIndex == -1) {
        r = new Response(cname);
        cache.insert(*r);
      } else {
        r = &cache[cnameIndex];
        if(r->resolved) {
          cache[index].ipAddress = r->ipAddress;
          cache[index].resolved = true;
        }
      }

      cache[index].cname = r;
    } else {
      // Not an A record or a CNAME. Ignore.
    }

    free(assembled);

    return E_MDNS_OK;
  }

  MDNS_RESULT Answer::parseAnswer(unsigned char* buffer, unsigned int len, unsigned int* offset, Answer* answer) {
    char* assembled = (char *)malloc(sizeof(char) * MDNS_MAX_NAME_LEN);
    int nameLen = Answer::assembleName(buffer, len, offset, &assembled);

    if(nameLen == -1 * E_MDNS_POINTER_OVERFLOW) {
      free(assembled);
      return nameLen;
    }

    answer->name = (char *)malloc(sizeof(char) * nameLen);
    parseName(&answer->name, assembled, strlen(assembled));
    answer->type = (buffer[(*offset)++] << 8) + buffer[(*offset)++];

    unsigned int aclass = (buffer[(*offset)++] << 8) + buffer[(*offset)++];
    answer->cacheflush = aclass >> 15;
    answer->aclass = aclass & 0x7f;

    answer->ttl = (buffer[(*offset)++] << 24) + (buffer[(*offset)++] << 16) + (buffer[(*offset)++] << 8) + buffer[(*offset)++];
    answer->len = (buffer[(*offset)++] << 8) + buffer[(*offset)++];

    if(answer->type == MDNS_A_RECORD) {
      answer->data = (unsigned char *)malloc(sizeof(unsigned char) * answer->len);
      memcpy(answer->data, buffer + (*offset), answer->len);
      (*offset) += answer->len;
    } else if(answer->type == MDNS_CNAME_RECORD) {
      unsigned int dataOffset = (*offset);
      (*offset) += answer->len;
      int dataLen = Answer::assembleName(buffer, len, &dataOffset, &assembled, answer->len);

      if(dataLen == -1 * E_MDNS_POINTER_OVERFLOW) {
        free(assembled);
        return dataLen;
      }

      answer->data = (unsigned char *)malloc(sizeof(unsigned char) * (dataLen - 1));
      answer->len = dataLen - 1;

      // This will fragment...
      char* d = (char *)answer->data;
      parseName(&d, assembled, dataLen - 1);
    } else {
      // Not an A record or a CNAME. Ignore.
      answer->len = 0;
      answer->data = NULL;
    }

    free(assembled);

    return E_MDNS_OK;
  }

  // Response should have the name set already
  MDNS_RESULT Answer::buildResponse(unsigned char* buffer, unsigned int len, Response& response) {
    if((buffer[2] & 0b10000000) != 0b10000000) {
      // Not an answer packet
      return E_MDNS_OK;
    }

    if(buffer[2] & 0b00000010) {
      // Truncated - we don't know what to do with these
      return E_MDNS_TRUNCATED;
    }

    if (buffer[3] & 0b00001111) {
      return E_MDNS_PACKET_ERROR;
    }

    unsigned int answerCount = (buffer[6] << 8) + buffer[7];

    // For this library, we are only interested in packets that contain answers
    if(answerCount == 0) {
      return E_MDNS_OK;
    }

    unsigned int offset = 0;

    MDNS_RESULT questionResult = skipQuestions(buffer, len, &offset);
    if(questionResult != E_MDNS_OK) {
      return questionResult;
    }

    Answer* answers = new Answer[answerCount];
    for(int i = 0; i < answerCount; i++) {
      parseAnswer(buffer, len, &offset, answers + i);
    }

    delete[] answers;
    return E_MDNS_OK;
  }

  // Converts a encoded DNS name into a FQDN.
  // name: pointer to char array where the result will be stored. Needs to have already been allocated. It's allocated length should be len - 1
  // mapped: The encoded DNS name
  // len: Length of mapped
  MDNS_RESULT Answer::parseName(char** name, const char* mapped, unsigned int len) {
    unsigned int namePointer = 0;
    unsigned int mapPointer = 0;

    while(mapPointer < len) {
      int labelLength = mapped[mapPointer++];

      if(labelLength > 0x3f) {
        return E_MDNS_INVALID_LABEL_LENGTH;
      }

      if(namePointer + labelLength > len - 1) {
        return E_MDNS_PACKET_ERROR;
      }

      if(namePointer != 0) {
        (*name)[namePointer++] = '.';
      }

      for(int i = 0; i < labelLength; i++) {
        (*name)[namePointer++] = mapped[mapPointer++];
      }
    }

    (*name)[len - 1] = '\0';

    return E_MDNS_OK;
  }

  int Answer::assembleName(unsigned char *buffer, unsigned int len, unsigned int *offset, char **name, unsigned int maxlen) {
    unsigned int index = 0;
    unsigned int nameLength = 0;

    while(index < maxlen) {
      if((buffer[*offset] & 0xc0) == 0xc0) {
        unsigned int pointerOffset = ((buffer[(*offset)++] & 0x3f) << 8) + buffer[(*offset)++];
        if(pointerOffset > len) {
          // Points to somewhere beyond the packet
          return -1 * E_MDNS_POINTER_OVERFLOW;
        }

        char *namePointer = *name + nameLength;
        int pointerLen = assembleName(buffer, len, &pointerOffset, &namePointer, MDNS_MAX_NAME_LEN - nameLength);

        if(pointerLen < 0) {
          return pointerLen;
        }

        nameLength += pointerLen;

        break;
      } else if(buffer[*offset] == '\0') {
        (*name)[nameLength++] = buffer[(*offset)++];
        break;
      } else {
        (*name)[nameLength++] = buffer[(*offset)++];
      }
      index++;
    }

    return nameLength;
  }

  int Answer::assembleName(unsigned char *buffer, unsigned int len, unsigned int *offset, char **name) {
    return assembleName(buffer, len, offset, name, MDNS_MAX_NAME_LEN);
  }

  // Work out how many bytes are dedicated to questions. Since we aren't answering questions, they can be skipped
  // buffer: The mDNS packet we are parsing
  // len: Length of the packet
  // offset: the byte we are up to in the parsing process
  MDNS_RESULT Answer::skipQuestions(unsigned char* buffer, unsigned int len, unsigned int* offset) {
    unsigned int questionCount = (buffer[4] << 8) + buffer[5];

    *offset += 12;
    for(int i = 0; i < questionCount; i++) {

      while(buffer[*offset] != '\0') {
        // If it's a pointer, add two to the counter
        if((buffer[*offset] & 0xc0) == 0xc0) {
          (*offset) += 2;
          break;
        } else {
          unsigned int labelLength = (unsigned int)buffer[*offset];

          if(labelLength > 0x3f) {
            return E_MDNS_INVALID_LABEL_LENGTH;
          }

          (*offset) += 1; // Increment to move to the next byte
          (*offset) += labelLength;

          if(*offset > len) {
            return E_MDNS_PACKET_ERROR;
          }
        }
      }

      (*offset) += 5; // 2 bytes for the qtypes and 2 bytes qclass + plus one to land us on the next bit
    }

    if(*offset > len + 1) {
      return E_MDNS_PACKET_ERROR;
    }

    return E_MDNS_OK;
  }
};
