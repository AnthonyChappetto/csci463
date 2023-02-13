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
#ifndef MEMORY_H
#define MEMORY_H

#include "hex.h"

class memory : public hex
{
public:

/**********************************************************************************
 * Program's constructor
 * 
 * Allocates siz bytes in the mem vector and initializes every byte to 0xa5
 * Then calls the resize() method after being rounded up mod-16
 *
 * @param s The number of bytes to create in the simulated memory
 **********************************************************************************/ 
    memory(uint32_t siz);

/********************************************************
 * The destructor of the program
 * 
 * Cleans up anything necessary by calling clear()
 *******************************************************/ 
    ~memory();

/*******************************************************************************
 * Checks mem vector for legal elements
 * 
 * Returns true if i (holding an element) does not exist
 * in the vector mem. Prints out a warning message if this occurs
 *
 * @param i the element to check in the vector
 * 
 * @return True if i is not present in mem vector, false if i is present
 ******************************************************************************/ 
    bool check_illegal(uint32_t i) const;

/**************************************************************
 * Returns number of bytes in simulated memory
 * 
 * Rounds up the number of bytes in the simulated memory 
 * before returning it through the size() function
 *
 * @return The size of the memory vector
 *************************************************************/ 
    uint32_t get_size() const;

/**************************************************************************
 * Checks if the given address in memory is valid and returns it
 * 
 * Calls check_illegal() to check the given addr in mem and then
 * returns the value of the byte at the given address
 *
 * @param addr The address to be checked
 * 
 * @return 0 if the addr fails check_illegal() or it returns the addr
 * as a element of mem
 *************************************************************************/  
    uint8_t get8(uint32_t addr) const;

/*********************************************************************
 * Calls get8() twice to create 16-bit return value
 * 
 * Function calls get8() twice to use two bytes and combine
 * in little-endian order to create 16-bit return value
 *
 * @param addr The address to be checked
 * 
 * @return get8() in little endian order
 ********************************************************************/
    uint16_t get16(uint32_t addr) const;

/*****************************************************************
 * Calls get16() twice to create a 32-bi return value
 * 
 * Function calls get16() twice to use 4 bytes and combine
 * in little-endian order to create a 32bit return value
 *
 * @param addr The address to be checked
 * 
 * @return get16() in little endian order
 ****************************************************************/ 
    uint32_t get32(uint32_t addr) const;

 /************************************************************************
 * Calls get8() to return sign-extended value
 * 
 * Returns sign-extended value as 32-bit sign int by calling get8()
 *
 * @param addr The address to be checked
 * 
 * @return Sign-extended value as 32-bit signed int
 ************************************************************************/ 
    int32_t get8_sx(uint32_t addr) const;

/*************************************************************************
 * Calls get16() and returns sign-extended value of the byte
 * as a 32-bit signed int 
 *
 * @param addr The address to be checked
 * 
 * @return Sign-extended value of 16-bit value as 32-bit signed int
 ************************************************************************/ 
    int32_t get16_sx(uint32_t addr) const;

/*****************************************************************************************
 * Function calls get32() to return the value as a 32-bit signed int
 *
 * @param addr The address to be checked
 * 
 * @return Sign-extended value as a 32-bit signed int
 ****************************************************************************************/ 
    int32_t get32_sx(uint32_t addr) const;

/******************************************************************************************
 * Function uses check_illegal to verify addr, then set it to the byte in simulated
 * memory at the addr to the given val
 *
 * @param addr The address to be checked
 * @param val An element of the mem vector that addr will be set as
 *****************************************************************************************/
    void set8(uint32_t addr, uint8_t val);

/************************************************************************
 * Calls set8() twice to store given val in little-endian order
 *
 * @param addr The address to be checked
 * @param val An element of the mem vector that addr will be set as
 ***********************************************************************/ 
    void set16(uint32_t addr, uint16_t val);

/**********************************************************************
 * Calls set16() twice to store given val in little-endian order
 *
 * @param addr The address to be checked
 * @param val An element of the mem vector that addr will be set as
 *********************************************************************/ 
    void set32(uint32_t addr, uint32_t val);

/*********************************************************
 * Dumps entire contents of simulated memory in
 * hex with corresponding ASCII characters on the right
 ********************************************************/ 
    void dump() const;

/*******************************************************
 * Open file fname in binary mode and reads contents
 * into the simulated memory
 *
 * @param fname File to be opened
 * 
 * @return false if the file cannot be opened
 ******************************************************/ 
    bool load_file(const std::string &fname);

private:
    std::vector<uint8_t> mem;
};

#endif