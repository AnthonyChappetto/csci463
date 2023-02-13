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
#ifndef CPU_H
#define CPU_H

class cpu_single_hart : public rv32i_hart
{
public:

/*********************************************************************
 * Constructor that passes memrory class instance to
 * base class constructor
 * 
 * @param mem Memory to be passed through
 ********************************************************************/
	cpu_single_hart(memory& mem) : rv32i_hart(mem) {}

/*********************************************************************
 * Ensure the code can execute on the simulator by allocating enough
 * memory to the machine
 * 
 * @param exec_limit The limit on how many instructions can be
 * executed by the machine 
 ********************************************************************/		
	void run(uint64_t exec_limit);		
};


#endif