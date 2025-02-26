#include <iostream>
#include "BaseCache.h"
#include <cmath>
using namespace std;

//WRITE ME
//Default constructor to set everything to '0'
BaseCache::BaseCache() {
    // printf("\t-->default constructor\n");
    setCacheSize(0);
    setAssociativity(0);
    setBlockSize(0);
    // printf("\t-->end of default constructor\n");
}

//WRITE ME
//Constructor to initialize cache parameters, create the cache and clears it
BaseCache::BaseCache(uint32_t _cacheSize, uint32_t _associativity, uint32_t _blockSize) {
    // printf("\t-->secondary constructor\n");
    setCacheSize(_cacheSize);
    setAssociativity(_associativity);
    setBlockSize(_blockSize);
    
    initDerivedParams();
    createCache();
    resetStats();
    
    // printf("\t-->end of secondary constructor\n");
}

//WRITE ME
//Set cache base parameters
void BaseCache::setCacheSize(uint32_t _cacheSize) {
    cacheSize = _cacheSize;
}
void BaseCache::setAssociativity(uint32_t _associativity) {
    associativity = _associativity;
}
void BaseCache::setBlockSize(uint32_t _blockSize) {
    blockSize = _blockSize;
}

//WRITE ME
//Get cache base parameters
uint32_t BaseCache::getCacheSize() {
    return cacheSize;
}
uint32_t BaseCache::getAssociativity() {
    return associativity;
}
uint32_t BaseCache::getBlockSize() {
    return blockSize;
}

//WRITE ME
//Get cache access statistics
uint32_t BaseCache::getReadHits() {
    return numReadHits;
}
uint32_t BaseCache::getReadMisses() {
    return numReadMisses;
}
uint32_t BaseCache::getWriteHits() {
    return numWriteHits;
}
uint32_t BaseCache::getWriteMisses() {
    return numWriteMisses;
}

double BaseCache::getReadHitRate() {
    double result = 0;
    result = (numReadHits / numReads)*100;
    return result;
}
double BaseCache::getReadMissRate() {
    double result = 0;
    result = (numReadMisses / numReads)*100;
    return result;
}

double BaseCache::getWriteHitRate() {
    double result = 0;
    result = (numWriteHits / numWrites)*100;
    return result;
}
double BaseCache::getWriteMissRate() {
    double result = 0;
    result = (numWriteMisses / numWrites)*100;
    return result;
}

double BaseCache::getOverallHitRate() {
    double result = 0;
    result = ((numReadHits + numWriteHits) / (numWrites + numReads))*100;
    return result;
}
double BaseCache::getOverallMissRate() {
    double result = 0;
    result = ((numReadMisses + numWriteMisses) / (numWrites + numReads))*100;
    return result;
}

//WRITE ME
//Initialize cache derived parameters
void BaseCache::initDerivedParams() {
    numSets = cacheSize / (associativity * blockSize);

    indexBits = log2(numSets);
    offsetBits = log2(blockSize);

    tagBits = ADDR_BITS - indexBits - offsetBits;

    LRUvector = vector<vector<int>>(numSets,vector<int>(0,0));

    // // testing
    // for (size_t j = 0; j < LRUvector.size(); j++){
    //     for (size_t i = 0; i < LRUvector[j].size(); i++){
    //         // printf("%f ",LRUvector[j][i]);
    //         cout << LRUvector[j][i];
    //         // LRUvector[j,i] = 3;
    //     }
    //     printf("\n");
    // }

    // for (int j = 0; j < LRUvector.size(); j ++){
    //     for (int i = 0; i < LRUvector[j].size(); i++){
    //         printf("%f ",LRUvector[j,i]);
    //     }
    //     printf("\n");
    // }

    // printf("assoc: %i",associativity);
    // printf("sets: %i",numSets);

    // printf("num sets: %i, index bits: %i, offset bits: %i, tag bits: %i\n",numSets,indexBits, offsetBits, tagBits);
}

//WRITE ME
//Reset cache access statistics
void BaseCache::resetStats() {
}

//WRITE ME
//Create cache and clear it
void BaseCache::createCache() {
    // printf("\t-->creating cache\n");
    // because cacheLines is a pointer to a pointer, assume 2D matrix
    // is desired because easier to find way and set this way 
    // create

    // this helped me a lot
    // https://www.geeksforgeeks.org/how-to-declare-a-2d-array-dynamically-in-c-using-new-operator/
    
    // create bunch of pointers that have memory for the number of sets
    cacheLines = new cacheLine*[numSets];

    // for each new pointers created create data in the other direction for the length of associativity
    for (int i = 0; i < numSets; i++){
        cacheLines[i] = new cacheLine[associativity];
    }

    // now can reference the cache in 2D
    // x is set and y is associativity
    for (uint32_t x = 0; x < associativity; x++){
        for (int y = 0; y < numSets; y++){
            cacheLines[x][y].tag = 0;
            cacheLines[x][y].data = NULL;
            cacheLines[x][y].valid = false;
        }
    }
    
    // clearCache();
}

//WRITE ME
//Reset cache
void BaseCache::clearCache() {
    delete[] cacheLines;
}

int BaseCache::give_replace_index(uint32_t index_bits){
    // printf("in function\n");

    int LRU;

    // if still empty
    if (LRUvector[index_bits].empty()){
        // printf("LRU is empty!!!\n");
        LRU = 0;
    }else{
        LRU = LRUvector[index_bits].front();
    }

    return LRU;


    // // given a set, return the least recently used index
    // // just grab the first item in a specific row from the vector
    // int LRU;
    // for (size_t j = 0; j < LRUvector.size(); j++){
    //     // if (!LRUvector[j].empty()){
    //     //     printf("not empty\n");
    //     // }else{
    //     //     printf("empty\n");
    //     // }
    //     cout << "j is " << j << endl;
    //     }

    // int LRUsize = LRUvector[index_bits].size();
    // printf("LRUsize = %i\n",LRUsize);
    // LRU = LRUvector[index_bits].front();
    // printf("LRU return: %i\n",LRU);
    // return LRU;

    // testing

    // cout << "size is " << LRUvector.size();

    // for (size_t j = 0; j < LRUvector.size(); j++){
    //     if (LRUvector[j].empty()){
    //         printf("size is zero");
    //     }
    //     // for (size_t i = 0; i < LRUvector[j].size(); i++){
    //     //     // printf("%f ",LRUvector[j][i]);
    //     //     cout << LRUvector[j][i];
    //     //     // LRUvector[j,i] = 3;
    //     // }
    //     printf("\n");
    // }
}

void BaseCache::updateLRU(uint32_t index_bits, int new_way){
    // given a set
    // if it's empty or not full, just add the position
    if ((LRUvector[index_bits].empty()) || LRUvector[index_bits].size() < associativity ){
        // printf("LRU MATRIX NOT FULL:%i to MRU\n",new_way);
        LRUvector[index_bits].push_back(new_way);
    // if it's full, put the position in the MRU spot
    }else if(LRUvector[index_bits].size() == associativity){
        // remove the first thing (LRU)
        LRUvector[index_bits].erase(LRUvector[index_bits].begin());
        // add the latest MRU
        // printf("LRU MATRIX FULL: LRU REMOVED, %i to MRU\n",new_way);
        LRUvector[index_bits].push_back(new_way);
    }
        // cout << "Current LRU vector: ";

    //     for (size_t j = 0; j < LRUvector.size(); j++){
    //         for (size_t i = 0; i < LRUvector[j].size(); i++){
    //             cout << LRUvector[j][i];
    //     }
    // }
        // printf("\n");
}

void BaseCache::evictBlock(uint32_t index_bits, int position){
    printf("evict block\n");
}

//WRITE ME
//Read data
//return true if it was a hit, false if it was a miss
//data is only valid if it was a hit, input data pointer
//is not updated upon miss. Make sure to update LRU stack
//bits. You can choose to separate the LRU bits update into
// a separate function that can be used from both read() and write().
bool BaseCache::read(uint32_t addr, uint32_t *data) {
    uint32_t tag = getTag(addr);
    uint32_t index = getIndex(addr); // which set you are on
    // uint32_t offset = getOffset(addr);

    // cout << "TAG: " << tag << " OFFSET:" << offset << " INDEX:" << index << endl;

    bool hit = false;
    uint32_t how_full = 0;

    // write to a line that is invalid or 
    for (uint32_t j = 0; j < associativity; j++){

        if (cacheLines[index][j].valid == true){
            // see how full the cache is
            how_full++;
        }

        if ((cacheLines[index][j].tag == tag) && (cacheLines[index][j].valid == true)){
            // printf("***READ HIT***\n");
            hit = true;

            // if you found the existing tag and its valid, find address of the data
            data = cacheLines[index][j].data;
            
            // make sure to update LRU
            updateLRU(index,j);
        }
    }

    // printf("CACHE STATUS: Index: %i, Valid Entries: %i\n",index,how_full);

        if (!hit){
            // printf("***READ MISS***\n");
            hit = false;
            int way = 0;

            // if its full, need to figure out which one to modify (tag only)
            if (how_full == associativity){
                // printf("find\n");
                way = give_replace_index(index);

                // on evict tag
                // printf("evict\n");
                cacheLines[index][way].tag = 0;
                cacheLines[index][way].valid = false;
            // if its not full, its just the next 
            } else{
                way = how_full;
            }

            // add the new data to evicted ones spot
            // printf("add the data to position %i\n",way);
            cacheLines[index][way].tag = tag;
            cacheLines[index][way].valid = true;
            
            // make sure to update LRU
            // printf("update LRU\n");
            updateLRU(index,way);

        }
        

        numReads++;
        if (hit){
            numReadHits++;
        }else{
            numReadMisses++;
        }


    return hit;

    // uint32_t tag = getTag(addr);
    // uint32_t index = getIndex(addr); // which set you are on
    // uint32_t offset = getOffset(addr);

    // // check the specific index
    // for (uint32_t j = 0; j < associativity; j++){
    //     if ((cacheLines[j][index].tag == tag)){
    //         printf("***READ HIT***\n");
    //         // make sure to update LRU
    //         cout << "tag is " << cacheLines[index][j].tag << endl; 
    //         printf("found data!\n");
    //     }
    //     else{
    //         printf("***READ MISS***\n");
    //         // update LRU if valid
    //         // do nothing if invalid 
    //         printf("update pointer??\n");
    //         break;
    //     }
    // }
}

//WRITE ME
//Write data
//Function returns write hit or miss status. 
bool BaseCache::write(uint32_t addr, uint32_t data) {


    uint32_t tag = getTag(addr);
    uint32_t index = getIndex(addr); // which set you are on
    // uint32_t offset = getOffset(addr);

    // cout << "TAG: " << tag << " OFFSET:" << offset << " INDEX:" << index << endl;

    bool hit = false;
    uint32_t how_full = 0;

    // write to a line that is invalid or 
    for (uint32_t j = 0; j < associativity; j++){

        if (cacheLines[index][j].valid == true){
            // see how full the cache is
            how_full++;
        }

        if ((cacheLines[index][j].tag == tag) && (cacheLines[index][j].valid == true)){
            // printf("***WRITE HIT***\n");
            hit = true;

            // if you found the existing tag and its valid, then replace the data only
            cacheLines[index][j].data = &data;
            
            // make sure to update LRU
            updateLRU(index,j);
        }
    }

    // printf("CACHE STATUS: Index: %i, Valid Entries: %i\n",index,how_full);

        if (!hit){
            // printf("***WRITE MISS***\n");
            hit = false;
            int way = 0;

            // if its full, need to figure out which one to get rid of
            if (how_full == associativity){
                // printf("find\n");
                way = give_replace_index(index);

                // actually evict it
                // printf("evict\n");
                cacheLines[index][way].tag = 0;
                cacheLines[index][way].data = nullptr;
                cacheLines[index][way].valid = false;
            // if its not full, its just the next 
            } else{
                way = how_full;
            }

            // add the new data to evicted ones spot
            // printf("add the data to position %i\n",way);
            cacheLines[index][way].tag = tag;
            cacheLines[index][way].data = &data;
            cacheLines[index][way].valid = true;
            
            // make sure to update LRU
            // printf("update LRU\n");
            updateLRU(index,way);

        }

            numWrites++;
        if (hit){
            numWriteHits++;
        }else{
            numWriteMisses++;
        }

    return hit;
}

uint32_t BaseCache::getTag(uint32_t addr){
    int ind_off = indexBits + offsetBits;
    uint32_t shifted = addr >> ind_off;
    return shifted;
}

uint32_t BaseCache::getIndex(uint32_t addr){
    int ind_off = offsetBits;
    uint32_t shifted = addr >> ind_off;
    // printf("index bits: %i\n",indexBits);
    uint32_t shiftBits =  (1 << indexBits) - 1;
    uint32_t index_bits = shifted & shiftBits;
    return index_bits;
}

uint32_t BaseCache::getOffset(uint32_t addr){
    // printf("offset bits: %i\n",offsetBits);
    uint32_t shiftBits =  (1 << offsetBits) - 1;
    uint32_t offset_bits = addr & shiftBits;
    return offset_bits;
}

//WRITE ME
//Destructor to free all allocated memeroy.
BaseCache::~BaseCache() {
}


// int main(int argc, char **argv) {
//     // printf("hello\n");
//     // //1024 byte cache, 4-way associativity, 64 byte block size
//     // BaseCache another(1024,4,64);

//     // uint32_t addr = 0x9876abca;
//     // uint32_t data = 0x349823ab;

//     // bool flag = another.write(addr,data);

//     // bool flagss = another.read(addr,&data);
// }