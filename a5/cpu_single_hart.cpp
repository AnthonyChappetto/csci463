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
#include "hex.h"
#include "rv32i_decode.h"
#include "rv32i_hart.h"
#include "cpu_single_hart.h"

void cpu_single_hart::run(uint64_t exec_limit) {
	regs.set(2, mem.get_size()); //Set the registers 

	while((exec_limit == 0 && is_halted() == false) || (get_insn_counter() < exec_limit && is_halted() == false))
		tick(); //Calls tick() while exec_limit == 0 and is_halted() is false or get_insn_counter() is less than exec_limit and is_halted() is false

	if(is_halted())
		cout << "Execution terminated. Reason: " << get_halt_reason() << endl; //Print out if execution failed and reason for

	cout << get_insn_counter() <<  " instructions executed" << endl; //Print out number of instructions executed
}