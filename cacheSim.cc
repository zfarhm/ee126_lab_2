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

uint32_t MemoryHierarchy::find_in_main_memory(uint32_t addr){
	uint32_t add_index;
	add_index = get_my_index(addr);
	uint32_t memory;
	memory = mainMemory[add_index];
	return memory;
}

void MemoryHierarchy::write_to_main_memory(uint32_t addr, uint32_t data){
	// printf("\tUPDATING MAIN MEMORY\n");
	uint32_t add_index;
	add_index = get_my_index(addr);
	mainMemory[add_index] = data;
}


int main(int argc, char **argv) {

	int minimum_time = 0;
	int maximum_time = 0;
	int total_time = 0;
	int read_time = 0;

	int L1_time = 1;
	int L2_time = 15;
	int mem_time = 700;


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
				cout <<"L1 write hit at 0x"<<hex<<address<<"\tData: 0x" <<hex<< data<<"\n";
				// need to push to L2 and main mem
				L2Cache.write(address,data);
				memH.write_to_main_memory(address, data);

				total_time = total_time + L1_time + L2_time + mem_time;

			} else {
				if (L2Cache.write(address,data)){
					// write hit L2
					cout <<"L2 write hit at 0x"<<hex<<address<<"\tData: 0x" <<hex<< data<<"\n";
					// L1Cache.write(address,data);
					memH.write_to_main_memory(address, data);
					total_time = total_time + L1_time + L2_time + mem_time;
				}else{
					// write miss L2
					cout <<"L1 and L2 write miss at 0x"<<hex<<address<<"\tData: 0x" <<hex<< data<<"\n";
					// NO WRITE ALLOCATE
					// dont update cache, just main memory
					// L2Cache.write_thru(address,data);
					memH.write_to_main_memory(address, data);
					total_time = total_time + L1_time + L2_time + mem_time;
				}
			}
	     }
		 // if a read
	    if (!(command.compare("r"))) { 
			// check the L1 cache
			if(L1Cache.read(address, &data)) {
				// read hit L1
				cout <<"L1 read hit at 0x"<<hex<<address<<"\tData: 0x" <<hex<< data<<"\n";
				total_time = total_time + L1_time;
				read_time = read_time + L1_time;

				// no action needed
			} else {
				if (L2Cache.read(address, &data)){
					// read hit L2
					cout <<"L2 read hit at 0x"<<hex<<address<<"\tData: 0x" <<hex<< data<<"\n";
					L1Cache.write_thru_miss(address,data);
					total_time = total_time + L1_time + L2_time;
					read_time = read_time + L1_time + L2_time;
				}else{
					// read miss L2
					cout <<"L1 and L2 read miss at 0x"<<hex<<address<<"\tData: 0x" <<hex<< data<<"\n";
					// search main memory for the data
					uint32_t data = memH.find_in_main_memory(address);
					// fill in L2 and L1 now 
					L2Cache.write_thru_miss(address,data);
					L1Cache.write_thru_miss(address,data);

					total_time = total_time + L1_time + L2_time + mem_time;
					read_time = read_time + L1_time + L2_time + mem_time;
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
	printf("Average memory access time (AMAT) (Reads): %i\n",read_time);

	// memory timing stats

    return 1;
}
