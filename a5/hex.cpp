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

std::string hex::to_hex8(uint8_t i) {
    ostringstream os; //declare an output stringstream
    os << std::hex << setfill('0') << setw(2) << static_cast<uint16_t>(i); //Use hex to convert 8 bits, fill 0's and static cast unsigned 16bit int
    return os.str(); //return os through function str()
}

std::string hex::to_hex32(uint32_t i)  {
    ostringstream os; //declare an output stringstream
    os << std::hex << setfill('0') << setw(8) << i; //Use hex to convert 8 bits, fill 0's and return as a str()
    return os.str(); //return os through function str()
}

std::string hex::to_hex0x32(uint32_t i) {
	return string("0x")+to_hex32(i); //return as a string calling to_hex32
}

std::string hex::to_hex0x20(uint32_t i) {
	ostringstream os; //declare an output stringstream
	os << string("0x") << std::hex << setfill('0') << setw(5) << (i >> 12); //Use hex to convert 5 bits and fill with 0's
	return os.str(); //return os through function str()
}

std::string hex::to_hex0x12(uint32_t i) {
	ostringstream os; //declare an output stringstream
	os << string("0x") << std::hex << setw(3) << setfill('0') << (i & 0x0fff); //Use hex to to convert 3 bits
	return os.str(); //return os through function str()
}