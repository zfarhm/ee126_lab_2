#include <iostream>
#include "BaseCache.h"
#include <cmath>
using namespace std;

//WRITE ME
//Default constructor to set everything to '0'
BaseCache::BaseCache() {
    testMode = false;

    setCacheSize(0);
    setAssociativity(0);
    setBlockSize(0);
    resetStats();
}

//WRITE ME
//Constructor to initialize cache parameters, create the cache and clears it
BaseCache::BaseCache(uint32_t _cacheSize, uint32_t _associativity, uint32_t _blockSize) {
    testMode = true;

    setCacheSize(_cacheSize);
    setAssociativity(_associativity);
    setBlockSize(_blockSize);

    initDerivedParams();
    createCache();
    resetStats();
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
double BaseCache::getReadHits() {
    return numReadHits;
}
double BaseCache::getReadMisses() {
    return numReadMisses;
}
double BaseCache::getWriteHits() {
    return numWriteHits;
}
double BaseCache::getWriteMisses() {
    return numWriteMisses;
}

double BaseCache::getReadHitRate() {
    double result;
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

    // initialize LRU vector/matrix
    // initialize all to 0
    LRUvector = vector<vector<int>>(numSets,vector<int>(associativity,0));

    // create base with 0 --> # ways for each set
    for (size_t j = 0; j < LRUvector.size(); j++){
        for (size_t i = 0; i < LRUvector[j].size(); i++){
            LRUvector[j][i] = i;
        }
    }

    if (testMode){
        printf("num sets: %i, index bits: %i, offset bits: %i, tag bits: %i\n",numSets,indexBits, offsetBits, tagBits);
    }
}

//WRITE ME
//Reset cache access statistics
void BaseCache::resetStats() {
    numReads = 0;
    numWrites = 0;
    numReadHits = 0;
    numReadMisses = 0;
    numWriteHits = 0;
    numWriteMisses = 0;
}

void BaseCache::print_cache_valid(){
    cout << "Current Cache: " << endl;
    for (int x = 0; x < numSets; x++){
        printf("%i: ",x);
        for (uint32_t y = 0; y < associativity; y++){
            if (cacheLines[x][y].valid){
                printf(" * ");
            }else{
                printf(" X ");
            } 
        }
        printf("\n");
    }
    printf("\n");
}

void BaseCache::print_LRU_matrix(){
    cout << "Current LRU vector: " << endl;
    for (size_t j = 0; j < LRUvector.size(); j++){
        printf("%li: ",j);
        for (size_t i = 0; i < LRUvector[j].size(); i++){
            cout << LRUvector[j][i];
    }            printf("\n");
    }
    printf("\n");
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

    // also this for vectors
    // https://www.digitalocean.com/community/tutorials/2d-vectors-in-c-plus-plus
    
    // 
    // 
    // for (int i = 0; i < numSets; i++){
    //     cacheLines[i] = new cacheLine[associativity];
    // }

    // create bunch of pointers that have memory for the number of sets
    cacheLines = new cacheLine*[numSets];

    // for each new pointers created create data in the other direction for the length of associativity
    for (int i = 0; i < numSets; i++){
        // allocate each row to be "associativity" length cacheLines
        cacheLines[i] = new cacheLine[associativity];
    }
    // now go through and for each row create the columns
    // where a struct is stored and memory properly allocated to hold data for the cache
    for (int i = 0; i < numSets; i++){
        for (uint32_t j = 0; j < associativity; j++){
            // for each 
            cacheLines[i][j].data = new uint32_t[blockSize];
    }
}

    // make sure valid and tag are set
    for (int i = 0; i < numSets; i++){
        for (uint32_t j = 0; j < associativity; j++){
            cacheLines[i][j].tag = 0;
            cacheLines[i][j].valid = false;
        }
    }
    
    // clearCache();
}

//WRITE ME
//Reset cache
void BaseCache::clearCache() {
    delete[] cacheLines;
}

int BaseCache::get_LRU_way(uint32_t index_bits){
    // printf("in function\n");

    int LRU;

    // if still empty
    if (LRUvector[index_bits].empty()){
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


int BaseCache::LRU_miss_extract(uint32_t index_bits){
    // gets the value and moves it to the back

    // find the current LRU value
    int LRU = 0;
    // if still empty
    if (LRUvector[index_bits].empty()){
        LRU = 0;
    }else{
        LRU = LRUvector[index_bits].front();
    }
        // remove first entry (LRU)
        LRUvector[index_bits].erase(LRUvector[index_bits].begin());
        // add to the back
        LRUvector[index_bits].push_back(LRU);

    if(testMode){
            print_LRU_matrix();
        }
    return LRU;
}

void BaseCache::LRU_hit_move(uint32_t index_bits, int way ){
    // find the value at the index
    int LRU_index = 0;
    for (int j = 0; j < associativity; j++){
        if (LRUvector[index_bits][j] == way){
            LRU_index = j;
        }
    }
        // remove the value at the index
        LRUvector[index_bits].erase(LRUvector[index_bits].begin() + LRU_index);

        // add to the back
        LRUvector[index_bits].push_back(way);
    if(testMode){
    print_LRU_matrix();
}
}

void BaseCache::updateMRU_adding(uint32_t index_bits, int new_way){
    // given a set
    // if it's empty or not full, just add the position
    // if ((LRUvector[index_bits].empty()) || LRUvector[index_bits].size() < associativity ){
        // printf("LRU MATRIX NOT FULL:%i to MRU\n",new_way);
        LRUvector[index_bits].push_back(new_way);
    // if it's full, put the position in the MRU spot

    // }

}

void BaseCache::updateLRU_static(uint32_t index_bits, int used_way){
        // given a set
    // if it's empty or not full, just add the position
    // if(LRUvector[index_bits].size() == associativity){
        // remove the first thing (LRU)
        if (!LRUvector[index_bits].empty()){
        LRUvector[index_bits].erase(LRUvector[index_bits].begin());
        // add the latest MRU
        LRUvector[index_bits].push_back(used_way);
    }else{
        LRUvector[index_bits].push_back(used_way);
    }

    // }


}

void BaseCache::evictBlock(uint32_t index, int way){
    cacheLines[index][way].tag = 0;
    cacheLines[index][way].valid = false;

    delete[] cacheLines[index][way].data;
    cacheLines[index][way].data = new uint32_t[blockSize];
    // cacheLines[index][way].data = nullptr;
}

void BaseCache::count_valids(uint32_t index){
    how_full = 0;

    for (uint32_t j = 0; j < associativity; j++){
        if (cacheLines[index][j].valid == true){
            how_full++;
        }
    }

    cout << "for index " << index << " counted " <<how_full << endl;

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
    uint32_t offset = getOffset(addr);

    if (testMode){
    cout << "TAG: " << tag << " OFFSET:" << offset << " INDEX:" << index << endl;
    }

    bool hit = false;
    int way = 0;
    int LRU = 0;

    // count_valids(index);

    // write to a line that is invalid or 
    for (uint32_t j = 0; j < associativity; j++){
        if ((cacheLines[index][j].tag == tag) && (cacheLines[index][j].valid == true)){
            // printf("***READ HIT***\n");
            hit = true;

            LRU_hit_move(index, j);

            // used https://www.geeksforgeeks.org/memcpy-in-cc/
            memcpy(&cacheLines[index][way].data[offset], data, sizeof(uint32_t));
            break;
        }
    }
        if (!hit){
            // printf("***READ MISS***\n");
            hit = false;

            
            LRU = LRU_miss_extract(index);

            // because its read miss, just update tag and LRU cache line
            // do not alter data
            cacheLines[index][LRU].tag = tag;
            cacheLines[index][LRU].valid = true;
        }
        
        numReads++;
        if (hit){
            numReadHits++;
        }else{
            numReadMisses++;
        }

        if (testMode){
    print_cache_valid();
}
    return hit;

}

//WRITE ME
//Write data
//Function returns write hit or miss status. 
bool BaseCache::write(uint32_t addr, uint32_t data) {


    uint32_t tag = getTag(addr);
    uint32_t index = getIndex(addr); // which set you are on
    uint32_t offset = getOffset(addr);

    if (testMode){
    cout << "TAG: " << tag << " OFFSET:" << offset << " INDEX:" << index << endl;
    }

    bool hit = false;
    int way = 0;
    int LRU = 0;

    // count_valids(index);

    // write to a line that is invalid or 
    for (uint32_t j = 0; j < associativity; j++){
        if ((cacheLines[index][j].tag == tag) && (cacheLines[index][j].valid == true)){
            // printf("***WRITE HIT***\n");
            hit = true;

            LRU_hit_move(index, j);

            cacheLines[index][j].tag = tag;
            cacheLines[index][j].valid = true;

            // used https://www.geeksforgeeks.org/memcpy-in-cc/
            memcpy(&cacheLines[index][j].data[offset], &data, sizeof(uint32_t));
            
            // make sure to update LRU
            break;
        }
    }

        if (!hit){
            // printf("***WRITE MISS***\n");
            hit = false;

            LRU = LRU_miss_extract(index);

            // if there is data in there, evict it
            if (cacheLines[index][way].valid){
                evictBlock(index,LRU);
            }

            // add the new data to evicted ones spot
            // printf("add the data to position %i\n",way);
            cacheLines[index][LRU].tag = tag;
            cacheLines[index][LRU].valid = true;

            // used https://www.geeksforgeeks.org/memcpy-in-cc/
            memcpy(&cacheLines[index][LRU].data[offset], &data, sizeof(uint32_t));

            // make sure to update LRU
            // printf("update LRU\n");
        }

            numWrites++;
        if (hit){
            numWriteHits++;
        }else{
            numWriteMisses++;
        }
        if (testMode){
            print_cache_valid();
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