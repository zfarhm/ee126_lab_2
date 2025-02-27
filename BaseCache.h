#ifndef __BASE_CACHE_H__
#define __BASE_CACHE_H__

#include <iostream>
#include <vector>
#include <bitset>
#include <sstream>
#include <cstring>

using namespace std;

#define ADDR_BITS 32
typedef struct _cacheLine {
  uint32_t tag;
  uint32_t *data; // data points to data stored in 
    		  // cache line. cache line size is
    		  // multiple of 32bits.
  bool valid;
} cacheLine;

class BaseCache{
  private:
    //cache base parameters
  	uint32_t cacheSize; //in Bytes
  	uint32_t associativity;
  	uint32_t blockSize;  //in Bytes
    cacheLine** cacheLines;

	vector<vector<int>> LRUvector; // empty vector to track LRU
	
   	//cache derived parameters
    //define any additional parameters like number of
    //sets, index/tag bits or whatever is needed to
    //implement correct cache behavior.
	int numSets;
	int indexBits;
	int offsetBits;
	int tagBits;
	  //WRITE ME

	  //cache access statistics
	  double numReads;
	  double numWrites;
	  double numReadHits;
	  double numReadMisses;
	  double numWriteHits;
	  double numWriteMisses;

  public:

	bool testMode;

    //Default constructor to set everything to '0'.
    BaseCache();        
    //Constructor to initialize cache parameters and to create the cache
    BaseCache(uint32_t _cacheSize, uint32_t _associativity, uint32_t _blockSize);
	  //Set cache base parameters

	void print_cache_valid();

    void setCacheSize(uint32_t _cacheSize); 
    void setAssociativity(uint32_t _associativity);
    void setBlockSize(uint32_t _blockSize);
        
    //Get cache base parameters
	  uint32_t getCacheSize(); 
    uint32_t getAssociativity(); 
    uint32_t getBlockSize(); 
	
	  //Get cache access statistics
	  double getReadHits(); 
	  double getReadMisses(); 
	  double getWriteHits(); 
	  double getWriteMisses(); 

	  double getReadHitRate();
	  double getReadMissRate();
	  double getWriteHitRate();
	  double getWriteMissRate();

	  double getOverallHitRate();
	  double getOverallMissRate();

	  //Initialize cache derived parameters
    //This function computes the derived parameters given
    //the base parameters.
	  void initDerivedParams();
	  //Reset cache access statistics
	  void resetStats();
	  //Create cache and clear it
	  void createCache();
	  //Reset cache
	  void clearCache();

	int get_LRU_way(uint32_t index_bits);
	void updateLRU(uint32_t index_bits, int new_way);
	void evictBlock(uint32_t index_bits,int position);

	  //Read data
	  //return true if it was a hit, false if it was a miss
	  //data is only valid if it was a hit, input data pointer
	  //is not updated upon miss.
	  bool read(uint32_t addr, uint32_t *data);
	  //Write data
	  //Function returns write hit or miss status. 
	  bool write(uint32_t addr, uint32_t data);

	  /********ADD ANY ADDITIONAL METHODS IF REQUIRED*********/	 
	  uint32_t getTag(uint32_t addr);
	  uint32_t getIndex(uint32_t addr);
	  uint32_t getOffset(uint32_t addr);
	   
	  //Destructor to free all allocated memeroy.
	  ~BaseCache();
};
#endif
