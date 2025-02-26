#include <iostream>
#include "BaseCache.h"
#include <cmath>
using namespace std;

//WRITE ME
//Default constructor to set everything to '0'
BaseCache::BaseCache() {
}

//WRITE ME
//Constructor to initialize cache parameters, create the cache and clears it
BaseCache::BaseCache(uint32_t _cacheSize, uint32_t _associativity, uint32_t _blockSize) {
}

//WRITE ME
//Set cache base parameters
void BaseCache::setCacheSize(uint32_t _cacheSize) {}
void BaseCache::setAssociativity(uint32_t _associativity) {}
void BaseCache::setBlockSize(uint32_t _blockSize) {}

//WRITE ME
//Get cache base parameters
uint32_t BaseCache::getCacheSize() {}
uint32_t BaseCache::getAssociativity() {}
uint32_t BaseCache::getBlockSize() {}

//WRITE ME
//Get cache access statistics
uint32_t BaseCache::getReadHits() {}
uint32_t BaseCache::getReadMisses() {}
uint32_t BaseCache::getWriteHits() {}
uint32_t BaseCache::getWriteMisses() {}
double BaseCache::getReadHitRate() {}
double BaseCache::getReadMissRate() {}
double BaseCache::getWriteHitRate() {}
double BaseCache::getWriteMissRate() {}
double BaseCache::getOverallHitRate() {}
double BaseCache::getOverallMissRate() {}

//WRITE ME
//Initialize cache derived parameters
void BaseCache::initDerivedParams() {
}

//WRITE ME
//Reset cache access statistics
void BaseCache::resetStats() {
}

//WRITE ME
//Create cache and clear it
void BaseCache::createCache() {
}

//WRITE ME
//Reset cache
void BaseCache::clearCache() {
}

//WRITE ME
//Read data
//return true if it was a hit, false if it was a miss
//data is only valid if it was a hit, input data pointer
//is not updated upon miss. Make sure to update LRU stack
//bits. You can choose to separate the LRU bits update into
// a separate function that can be used from both read() and write().
bool BaseCache::read(uint32_t addr, uint32_t *data) {
}

//WRITE ME
//Write data
//Function returns write hit or miss status. 
bool BaseCache::write(uint32_t addr, uint32_t data) {
}

//WRITE ME
//Destructor to free all allocated memeroy.
BaseCache::~BaseCache() {
}

