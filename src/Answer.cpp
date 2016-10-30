#include "Answer.h"

namespace mDNSResolver {
  Answer::Answer() {}

  MDNS_RESULT Answer::parse(unsigned char* buffer, unsigned int len) {
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
    return E_MDNS_OK;
    //for(int i = 0; i < answerCount; i++) {
      //Answer answer();

      //if(parseAnswer(buffer, len, &offset, &answer) == E_MDNS_OK) {
        //int resultIndex = search(answer.name);
        //if(resultIndex != -1) {
          //if(answer.type == 0x01) {
            //_cache[resultIndex].ipAddress = IPAddress((int)answer.data[0], (int)answer.data[1], (int)answer.data[2], (int)answer.data[3]);
            //_cache[resultIndex].ttl = answer.ttl;
            //_cache[resultIndex].waiting = false;
          //} else if(answer.type == 0x05) {
            //// If data is already in there, copy the data
            //int cnameIndex = search((char *)answer.data);

            //if(cnameIndex != -1) {
              //_cache[resultIndex].ipAddress = _cache[cnameIndex].ipAddress;
              //_cache[resultIndex].waiting = false;
              //_cache[resultIndex].ttl = answer.ttl;
            //} else {
              //Response r = buildResponse((char *)answer.data);
              //insert(r);
            //}
          //}
        //}

        //free(answer.data);
        //free(answer.name);
        //return E_MDNS_OK;
      //} else {
        //if(answer.data) {
          //free(answer.data);
        //}
        //if(answer.name) {
          //free(answer.name);
        //}
        //return E_MDNS_PARSING_ERROR;
      //}
    //}
  }

  // Work out how many bytes are dedicated to questions. Since we aren't answering questions, they can be skipped
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
          unsigned int labelSize = (unsigned int)buffer[*offset];

          if(labelSize > 0x3cf) {
            return E_MDNS_PACKET_ERROR;
          }

          (*offset) += 1; // Increment to move to the next byte
          (*offset) += labelSize;
        }
      }

      (*offset) += 5; // 2 bytes for the qtypes and 2 bytes qclass + plus one to land us on the next bit
    }

    return E_MDNS_OK;
  }
};
