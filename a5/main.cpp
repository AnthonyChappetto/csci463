//***************************************************************************
//
//  Anthony Chappetto
//  z1888754
//  CSCI 463 Section 0001
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hex.h"
#include "rv32i_decode.h"
#include "rv32i_hart.h"
#include "cpu_single_hart.h"

static void usage() {
    cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
    cerr << "    -d show disassembly before program execution" << endl;
    cerr << "    -i show instruction printing during execution" << endl;
    cerr << "    -l maximum number of instructions to exec" << endl;
    cerr << "    -m specify memory size (default = 0x100)" << endl;
    cerr << "    -r show register printing during execution" << endl;
    cerr << "    -z show a dump of the regs & memory after simulation" << endl;
    exit(1); //After printing out error for usage/d/i/l/m/r/z case, program crashes with a 1
}

static void disassemble(const memory &mem) {
  uint32_t i = 0;
  while(i < mem.get_size()) { 
      uint32_t insn = mem.get32(i); //sets insn to mem vector calling get32()
      cout << hex::to_hex32(i) << ": " 
           << hex::to_hex32(insn) << "  "; //calls hex functions 
      cout << rv32i_decode::decode(i, insn) << endl; //Calls decode
      i += 4;
  }	
}

int main(int argc, char **argv) {
    uint32_t memory_limit = 0x100; 
    int opt;
    int execution_limit, d, i, r, z = 0; //create flags for d,i,r, and z

    while((opt=getopt(argc, argv, "m:l:dirz"))!=-1) { 
        switch(opt) { //switch statement for each flag
            case 'm':
                memory_limit =stoul(optarg, nullptr, 16); //use stoul on the memory_limit
                break;
            case 'd':
                d=1; //set the dflag to 1
                break;
            case 'i':
                i=1; //set the iflag to 1
		        break;
            case 'r':
                r=1; //set the rflag to 1
                break;
            case 'z':
                z=1; //set the zflag to 1
                break;
            case 'l':
                execution_limit =stoul(optarg, nullptr, 0); //use stoul on the execution_limit
                break;
            default: //calls usage()
                usage();
        }
    }

    if(argc <= optind) 
        usage(); //call usage() is argc less than or equal to optind

    memory mem(memory_limit);

    if(!mem.load_file(argv[optind]))
        usage(); //call usage() if memory cannot load the file

    cpu_single_hart cpu(mem);
    rv32i_hart sim(mem);

    if(d==1) {
        sim.reset();
        disassemble(mem);   //reset and disassemble memory if d flag = 1        
    }

    if(i==1)
        cpu.set_show_instructions(true); //if i flag = 1 call cpu with set_show_instructions

    if(r==1)
        cpu.set_show_registers(true); //if r flag = 1 call cpu with set_show_instructions

    cpu.run(execution_limit);

    if(z==1) {
        cpu.dump(); //dump cpu and memory if z = 1
        mem.dump();
    }

    return 0;
}