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
#include <getopt.h>
#include "memory.h"
#include "rv32i_decode.h"

/***********************************************************
 * Function prints appropriate "Usage" error message
 * and "Pattern" to stderr and terminates the program
 **********************************************************/ 
static void usage()
{
  cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
  cerr << "    -m specify memory size (default = 0x100)" << endl;
  exit(1);
}

/*****************************************************************************
 * Function decodes and prints each instruction one at a time in a loop
 * 
 * @param mem The address of the memory 
 *****************************************************************************/
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

/******************************************************************************
 * Function utilizes functions from memory.cpp, hex.cpp, and rv32i_decode.cpp in
 * order to simulate a memory dump in ASCII 
 *
 * @param argc Command line argument
 * @param argv Command line argument
 * 
 * @return False if functions in memory.cpp, hex.cpp, and rv32i_decode.cpp do not work properly
 ******************************************************************************/ 
int main(int argc, char **argv) {
    uint32_t memory_limit = 0x100; // default memory size = 256 bytes
    int opt;
    while ((opt = getopt(argc, argv, "m:")) != -1) {
      switch (opt)
      {
      case 'm':
          {
            std::istringstream iss(optarg);
            iss >> std::hex >> memory_limit;
          }
          break;
        default: /* ’?’ */
          usage();
        }
    }

  if (optind >= argc)
      usage(); // missing filename

  memory mem(memory_limit);

  if (!mem.load_file(argv[optind]))
    usage();

  disassemble(mem);
  mem.dump();

  return 0;
}