#include "Cache.h"

namespace mDNSResolver {
  Cache::Cache() {
    this->size = 0;
  }

  Cache::~Cache() {
  }

  Response& Cache::operator[] (const int index) {
    return this->list[index];
  }

  int Cache::length() {
    return this->size;
  }

  void Cache::insert(Response response) {
    if(this->size == MDNS_RESOLVER_MAX_CACHE) {
      int i = oldest();
      remove(i);
    }

    this->list[this->size] = response;
    this->size += 1;
  }

  void Cache::remove(Response& response) {
    int i = search(response);
    if(i != -1) {
      remove(i);
    }
  }

  void Cache::expire() {
    int ttls[MDNS_RESOLVER_MAX_CACHE];

    for(int i = 0; i < this->size; i++) {
      this->list[i].decrementTTL();
      ttls[i] = this->list[i].ttl;
    }

    for(int i = 0; i < this->size; i++) {
      if(ttls[i] <= 0) {
        remove(i);
      }
    }
  }

  int Cache::search(Response& response) {
    for(int i = 0; i < this->size; i++) {
      if(this->list[i] == response) {
        return i;
      }
    }
    return -1;
  }

  int Cache::oldest() {
    if(this->size == 0) {
      return -1;
    }

    unsigned long oldestTTL = this->list[0].ttl;
    unsigned int oldestIndex = 0;

    for(int i = 1; i < this->size; i++) {
      if(this->list[i].ttl < oldestTTL) {
        oldestTTL = this->list[i].ttl;
        oldestIndex = i;
      }
    }

    return oldestIndex;
  }

  void Cache::remove(int index) {
    this->list[index] = this->list[this->size - 1];
    this->size -= 1;
  }
}
