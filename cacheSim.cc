#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BaseCache.h"
using namespace std;


void read_in_memory(){
	printf("try to read in memory\n");
	string inp_line, parsed_line;
	unsigned int address, data;
	ifstream fp_inp("memfoot.dat");
    if(fp_inp.is_open()) {
        while(getline(fp_inp, inp_line)) {
            stringstream inp_string(inp_line);
			if(getline(inp_string,parsed_line, ' ')) 
			address = stoul(parsed_line, NULL, 16);
			// cout << "ADDRESS " << address << endl;
			data = stoul(parsed_line, NULL, 16);
			// cout << "DATA " << data << endl;
        }
        fp_inp.close();
    }
	printf("successfully read in memory\n");
}


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

	read_in_memory();

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
	   
	    //Issue read/write command
		// if a write
	    if (!(command.compare("w"))) { 
	        if(L1Cache.write(address, data)) {
				// write hit
				// write data and address to all levels of cache and to main memory
	        } else {
				// write miss
				// no-write allocate for write misses
				// cache NOT updated with new address and data
				// no cache lines are evicted
				// only main memory is updated with new data for the address
	        }
	     }
		 // if a read
	    if (!(command.compare("r"))) { 
	        if(L1Cache.read(address, &data)) {
				// read hit
				// read hit results in no eviction/allocation
	        } else {
				// read miss
				// read the new block from lower levels into LRU cache line (or empty line if avail)
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
