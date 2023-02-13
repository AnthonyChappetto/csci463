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
#include "registerfile.h"
#include "hex.h"
#include <cstdint>
#include <iostream>
#include <iomanip>

using std::right;

void registerfile::reset() {
	rvReg[0]=0x0; //set reg0 to 0x0

	int i = 1;
	while(i<32) {
		rvReg[i] = 0xf0f0f0f0;	//While i is less than last register, set all registers to 0xf0f0f0f0
		i++;
	}
}

void registerfile::set(uint32_t r, int32_t val)
{
	if (r != 0) //if reg is not equal to 0
		rvReg[r] = val; //set registers to the val
	else
		return;
}


int32_t registerfile::get(uint32_t r) const
{
	if (r != 0) //if reg is not equal to 0
		return rvReg[r]; //retrieve the register and its val
	else
		return 0;
}

void registerfile::dump(const std::string &hdr) const
{
	size_t i = 0;
	while(i < 32) { //while i does not exceed reg 32
		if (i % 8==0 && i!=0) //if i divided by 8 = 0 and i is not 0
			cout << endl;
		if (i % 8 == 0) { //if i divided by 8 = 0 
			string x; //handles the registers divisable by 8
			if (i==24)
				x = "x24"; //set register to x24
			else if (i==16)
			    x = "x16"; //set register to x16
            else if(i==8)
                x = "x8"; //set register to x8
            else if(i==0)
                x = "x0"; //set register to x0
			cout << hdr << dec << right << setw(3) << setfill(' ') << x << " "; //print out register with dec/right/setw/setfill formatting
        }
		
        if(i!=0 && i % 8!=0 && i % 4==0) //if i is not 0 and i divided by 8 is not 0 and i divided by 4 is not 0
            cout << " " << hex::to_hex32(rvReg[i]) << " "; //print out register using to_hex32()
		else if (i % 8==7) //else i divided by 8 is 7
			cout << hex::to_hex32(rvReg[i]); //print out register using to_hex32()
        else
            cout << hex::to_hex32(rvReg[i]) << " "; 
		i++;
    }
	cout << endl;
}