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

using std::hex;
using std::ostringstream;

std::string hex::to_hex8(uint8_t i) {
    ostringstream os; //declare an output stringstream
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i); //Use hex to convert 8 bits, fill 0's, and static cast unsigned 16bit int
    return os.str(); //return os through function str()
}

std::string hex::to_hex32(uint32_t i) {
    ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << i; //Use hex to convert 8 bits, fill 0's, and static cast unsigned 16bit int
    return os.str();
}

std::string hex::to_hex0x32(uint32_t i) {
    return std::string("0x")+to_hex32(i);
}
