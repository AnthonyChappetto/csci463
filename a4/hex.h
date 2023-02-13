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
#ifndef HEX_H
#define HEX_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <unistd.h>
#include <fstream>

using std::ifstream;
using std::noskipws;
using std::vector;
using std::cerr;
using std::endl;
using std::cout;
using std::ostringstream;
using std::setw;
using std::setfill;
using std::left;
using std::string;

class hex
{
public:

/*****************************************************************************
 * Uses ostringstream to return a string with 2 hex digits representing
 * 8 bits of the i argument
 *
 * @param i Representation of 8 hex digits
 * 
 * @return String with exactly 2 hex digits representing the 8 bits
 * of the i argument
 *****************************************************************************/ 
    static std::string to_hex8(uint8_t i);

/**************************************************************************************
 * Function returnsa string with 8 bits representing the 32 bits of the i argument
 * 
 * @param i Representation of 8 hex digits
 * 
 * @return 8 hex digits
 **************************************************************************************/ 
    static std::string to_hex32(uint32_t i);

/***********************************************************************
 * Function returns 8 hex digits representing the 32
 * bits of the i argument through concatenating 0x and to_hex32()
 *
 * @param i Representation of 8 hex digits
 * 
 * @return 8 hex digits representing 32 bits
 **********************************************************************/ 
    static std::string to_hex0x32(uint32_t i);

/*****************************************************************************
 * Function returns a string with 0x followed by 5 hex digits
 * that represent the 20 lsb of the argument
 *
 * @param i Stores the 20 Least Significant Bits
 * 
 * @return
 *****************************************************************************/ 
    static std::string to_hex0x20(uint32_t i);

/*****************************************************************************
 * Function returns a string with 0x followed by 3 hex digits
 * that represent the 12 lsb of the argument
 *
 * @param i Stores the 12 Least Signifcant Bits
 * 
 * @return
 *****************************************************************************/   
    static std::string to_hex0x12(uint32_t i);
};

#endif     