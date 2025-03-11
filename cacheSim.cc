#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BaseCache.h"
using namespace std;

// struct memoryLine{
// 	uint32_t address;
// 	uint32_t data;
// }; 

MemoryHierarchy::MemoryHierarchy(){
	printf("construct\n");
}

MemoryHierarchy::~MemoryHierarchy(){
	printf("destruct\n");
}

void MemoryHierarchy::read_in_memory(){
	printf("try to read in memory\n");

	// mainMemory = vector<memoryLine>();

	string inp_line, parsed_line, address_str, data_str;
	unsigned int address, data;
	ifstream fp_inp("memfoot.dat");
    if(fp_inp.is_open()) {

        while(getline(fp_inp, inp_line)) {
            stringstream inp_string(inp_line);
			if(getline(inp_string,parsed_line)) {
				int index = parsed_line.find("\t");
				address_str = parsed_line.substr(0,index);
				// cout << "index is " << index << endl;
				address = stoul(address_str, NULL, 16);
				// uint32_t add_index = getIndex(address);
				uint32_t add_index = get_my_index(address);
				// cout << "ADDRESS " << address << endl;

				address_str = parsed_line.substr(index,-1);
				data = stoul(address_str, NULL, 16);
				// cout << "DATA " << data << endl;
				// cout << "ADD INDEX IS: " << add_index << endl;
				mainMemory[add_index] = data;
			}
			// mainMemory.push_back({address,data});
        }

        fp_inp.close();
    }
	printf("successfully read in memory\n");

	// for (int i = 0; i < (MAIN_MEMORY_SIZE/WORD_SIZE); i++){
	// 	cout << "address: " << i << " data: " << mainMemory[i] << endl;
	// }
}

uint32_t MemoryHierarchy::get_my_index(uint32_t addr){
	uint32_t add_index;
	add_index = addr / WORD_SIZE;
	return add_index;
}

uint32_t MemoryHierarchy::give_main_memory_data(uint32_t addr){
	uint32_t add_index;
	add_index = get_my_index(addr);
	uint32_t memory;
	memory = mainMemory[add_index];
	return memory;
}

void MemoryHierarchy::write_main_memory_data(uint32_t addr, uint32_t data){
	printf("\tUPDATING MAIN MEMORY\n");
	uint32_t add_index;
	add_index = get_my_index(addr);
	mainMemory[add_index] = data;
}

// size_t MemoryHierarchy::search_main_memory(uint32_t addr, ){
// 	// size_t index = 0;
// 	// printf("SEARCHING MAIN MEMORY....\n");
// 	// for (size_t i = 0; i < mainMemory.size(); i++){
// 	// 	if (mainMemory[i].address == addr){
// 	// 		printf("FOUND DATA IN MAIN MEMORY!\n");
// 	// 		index = i;
// 	// 		cout << "Address: "<< addr << " Index: " << i << endl;
// 	// 	}
// 	// }
// 	// return index;
// }


int main(int argc, char **argv) {
    string inp_line, parsed_line;
    string command;
    unsigned int address, data;

	// initialize L1 cache
    uint32_t cacheSize1, associativity1, blockSize1;
    cacheSize1 = atoi(argv[1]);
    associativity1 = atoi(argv[2]);
    blockSize1 = atoi(argv[3]);
    BaseCache L1Cache(cacheSize1, associativity1, blockSize1);

	// initialize L2 cache
	uint32_t cacheSize2, associativity2, blockSize2;
    cacheSize2 = atoi(argv[4]);
    associativity2 = atoi(argv[5]);
    blockSize2 = atoi(argv[6]);
    BaseCache L2Cache(cacheSize2, associativity2, blockSize2);

	// file is the last arg
	ifstream fp_inp(argv[7]);

	MemoryHierarchy memH;

	memH.read_in_memory();

    if(fp_inp.is_open()) {
        while(getline(fp_inp, inp_line)) {
            stringstream inp_string(inp_line);
    	    if(getline(inp_string,parsed_line, ' ')) 
		command = parsed_line;
	    if(getline(inp_string,parsed_line, ' ')) 
		address = stoul(parsed_line, NULL, 16);
	    if (!(command.compare("w"))) {
	    	if(getline(inp_string,parsed_line, ' ')) {
		    data = stoul(parsed_line, NULL, 16);
		}
	    }

		// cout << "ADDRESS IS " << address << endl;
	   
	    //Issue read/write command
		// if a write
	    if (!(command.compare("w"))) { 
			if(L1Cache.write(address, data)) {
				// write hit L1
				printf("L1 WRITE HIT\n");
				// need to push to L2 and main mem
				L2Cache.write_thru(address,data);
				memH.write_main_memory_data(address, data);

			} else {
				// L1 WRITE MISS

				if (L2Cache.write(address,data)){
					// write hit L2
					printf("L2 WRITE HIT\n");
					// L1Cache.write_thru(address,data);
					memH.write_main_memory_data(address, data);
				}else{
					// write miss L2
					printf("L2 WRITE MISS\n");
					// dont update cache, just main memory
					memH.write_main_memory_data(address, data);
				}
			}
	     }
		 // if a read
	    if (!(command.compare("r"))) { 
			// check the L1 cache
			if(L1Cache.read(address, &data)) {
				// read hit L1
				printf("L1 READ HIT\n");
				// no action needed
			} else {
				// read miss L1
				printf("L1 READ MISS\n");
				// L1 
				if (L2Cache.read(address, &data)){
					// read hit L2
					printf("L2 READ HIT\n");
					L1Cache.write_thru(address,data);
				}else{
					// read miss L2
					printf("L2 READ MISS\n");
					// search main memory
					// memH.search_main_memory(address);
					uint32_t data = memH.give_main_memory_data(address);
					// cout << "FOUND data is " << data << endl;

					// fill in L2 and L1 now 
					L2Cache.write_thru(address,data);
					L1Cache.write_thru(address,data);
				}

			}
	    }

        }
        fp_inp.close();
    }

	// L1
    cout <<endl;
    cout << "L1 Read Hits (HitRate): "<<L1Cache.getReadHits()<<" ("<<L1Cache.getReadHitRate()<<"%)"<<endl;
    cout << "L1 Read Misses (MissRate): "<<L1Cache.getReadMisses() <<" ("<<L1Cache.getReadMissRate()<<"%)"<<endl;
    cout << "L1 Write Hits (HitRate): "<<L1Cache.getWriteHits()<<" ("<<L1Cache.getWriteHitRate()<<"%)"<<endl;
    cout << "L1 Write Misses (MissRate): "<<L1Cache.getWriteMisses() <<" ("<<L1Cache.getWriteMissRate()<<"%)"<<endl;
    cout << "L1 Overall Hit Rate: "<<L1Cache.getOverallHitRate() <<"%" << endl;
    cout << "L1 Overall Miss Rate: "<<L1Cache.getOverallMissRate()<<"%"<<endl;

	// L2
	cout <<endl;
    cout << "L2 Read Hits (HitRate): "<<L2Cache.getReadHits()<<" ("<<L2Cache.getReadHitRate()<<"%)"<<endl;
    cout << "L2 Read Misses (MissRate): "<<L2Cache.getReadMisses() <<" ("<<L2Cache.getReadMissRate()<<"%)"<<endl;
    cout << "L2 Write Hits (HitRate): "<<L2Cache.getWriteHits()<<" ("<<L2Cache.getWriteHitRate()<<"%)"<<endl;
    cout << "L2 Write Misses (MissRate): "<<L2Cache.getWriteMisses() <<" ("<<L2Cache.getWriteMissRate()<<"%)"<<endl;
    cout << "L2 Overall Hit Rate: "<<L2Cache.getOverallHitRate() <<"%" << endl;
    cout << "L2 Overall Miss Rate: "<<L2Cache.getOverallMissRate()<<"%"<<endl;

	// memory timing stats

    return 1;
}
