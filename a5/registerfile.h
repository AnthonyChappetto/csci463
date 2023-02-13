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
#ifndef REG_H
#define REG_H

#include <stdint.h>
#include <string>

class registerfile
{
public:

/*********************************************************************
 * Constructor for the registerfile
 ********************************************************************/
    registerfile() { reset(); } 

/*********************************************************************
 * Initializes the register x0 to zero and all
 * other registers to 0xf0f0f0f0
 ********************************************************************/
    void reset();

/*********************************************************************
 * Assigns register r the given value
 *
 * @param r Register to be checked
 * @param val Value to be stored in register
 * 
 * @return Nothing if r iz zero
 ********************************************************************/
    void set(uint32_t r, int32_t val);

/*********************************************************************
 * Returns the value of register r
 *
 * @param r Register to be checked
 * 
 * @return 0 or the reg array containing r
 ********************************************************************/
    int32_t get(uint32_t r) const;

/*********************************************************************
 * Implements a dump of the registers.
 *
 * @param hdr String that must be printed at the beginning of
 * the output lines
 ********************************************************************/
    void dump(const std::string &hdr) const;

private:
    int32_t rvReg[32]; 
};

#endif