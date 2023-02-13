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
#include <iomanip>
#include <ostream>
#include <stdlib.h>
#include <cstdint>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

using std::cout;
using std::endl;
using std::cin;
using std::hex;
using std::setw;
using std::setfill;
using std::dec;

/**
 * Function prints out the first 4 lines of output requested per the assignment handout
 * 
 * Function utilizes a signed 32-bit integar value and an unsigned 32-bit integar value to represent the signifcand and exponent.
 * This is used to print out the first line that displays the hex value in binary groupings of 4 through division and logic gates.
 * The second line is then printed which utilizes an OR gate to determine if the sign bit is 1 or 0. The third line thenprints out
 * the exponent through binary to decimal conversion. Finally the fourth line prints out the signifcand in unsigned hex.
 *
 * @param x Used as a pointer to store signed and unsigned 32 bit integar values be printed in the output 
 * 
 * @return A pointer to the unsigned 32 and signed 32 bit value
 * 
 * @note Function exists as a way to make printBinFloat more simplified
 **************************************************************************************************************************************/
uint32_t printOutputLines(uint32_t x) {
    uint32_t unsignedSig = (x & 0x007fffff); //Unsigned 32 bit integar that represents the significand
    int32_t signedExponent = ((x & 0x7f800000) >> 23) -0x7F; //Signed 32 bit integar that repsents the input values exponent
    //set unsignedSig to bits 22 to 0 (discussed in class)
    //set signedExponent to bits 30 to 23 where the exponent is located

    //First Line: Displays input value as 32-bit hex value and in binary (groups of 4)
    cout << hex << "0x" << setw(8) << setfill('0') << x << " = "; //prints hex value adjusted to setw and with 0's to fill in
    for(int i = 31; i > -1; i--) { //loops backwards from 31 to obtain the corrct bits needed from each hex number 
        cout << (i < 31 && (i % 4) == 3 ? " " : "") << (x & 1 << i ? '1' : '0'); //prints the hex number in binary in groups of 4
    }
    cout << endl;

    //Second Line: Whether the sign bit is 0 or 1
    cout << "sign: " << (x &0x80000000 ? '1' : '0') << endl; //Code snipit from 'Hints' that explains to print 1 is most sig bit of x is 1 and 0 when 0.

    //Third Line: Prints what the exponent is
    cout << " exp: 0x" << setw(8) << signedExponent; //Prints out the exponent value
    cout << " (" << dec << signedExponent << ")" << hex << endl; //Converts the 32 bit signed int to decimal and prints out

    //Fourth Line: Prints the signifcand as a 32-bit unsigned hex value (0)
    cout << " sig: 0x" << setw(8) << setfill('0') << unsignedSig << endl; //prints signicand (bits 22 through 0)

    return x; 
}

/**
 * This function prints out the fifth and final line of output that is the floating point hex value in binary
 * 
 * This function utilizes a wide variety of logic gates to ensure that the sign of the hex value is negative or positve. It also checks
 * for infinity if the exponent is = 128 and the significand is = 0. It checks whetehr the exponent is positive or negative, and prints out
 * accordingly through specific loops and condional branches.
 *
 * @param x Used as a pointer to store signed and unsigned 32 bit integar values be printed in the output 
 * 
 * @return A pointer to the unsigned 32 and signed 32 bit value
 * 
 * @note Function exists as a way to make printBinFloat more simplified
 ***********************************************************************************************************************************************/
uint32_t printBinaryOutput(uint32_t x) {
    //re-declares variable locally
    uint32_t unsignedSig = (x & 0x007fffff);
    int32_t signedExponent = ((x & 0x7f800000) >> 23) -0x7F;

    //Fifth Line: the full value of the number printed in binary
    cout << (x &0x80000000 ? '-' : '+'); //conditional gate prints '-' if the sign is negative or '+' if the sign is positive
    
    //Begin checking for special + or - infinity case
    if(signedExponent == 128 && unsignedSig == 00000000) { //if the exponent is '11111111' AND significand is '00000000' (all ones and all zeros '11111111' in binary = 128 decimal)
        cout << "inf" << endl; 
    }
    else if(signedExponent == -127 && unsignedSig == 00000000) { //checks when exponent is -127 AND significand is all 0's
        cout << "0" << endl; //print 0 for special case
    }
    else if((int)signedExponent >= 0) { //Prints the binary float value when the exponent is positive
        int binaryValues = (int)signedExponent + 23;  //Initialize the digits to be converted to binary values and printed
        cout << '1'; //The 1 represents the leading signifcand digit
        unsignedSig = unsignedSig << 9; 

        for(int i = 0; i < binaryValues; i++) { //Loop reaches amount of digits needed for printing
            if(i == (int)signedExponent) { //when i (number of digits needed) == the exponent
                cout << '.';
                }
            cout << ((unsignedSig & 0x80000000) ? '1' : '0'); //Checks if the signifcand is 1
            unsignedSig = unsignedSig << 1; //Shifts bit value to the left

            if(i >= (int)signedExponent && i > 23) { //when i is = to or greater than the exponent in binary
                break; //Breaks logic gate to ensure no zeros are printed beyond the decimal
            }
        }
        cout << endl;
    }
    else if(signedExponent < 0) { //Prints the binary float value when the exponent is negative
        cout << "0.";
        unsignedSig = unsignedSig << 9; //Shift occurs to ensure the bits that make up the signifcand are correct

        int leadZeros = abs(signedExponent) - 1; //Variable stores how many zeros are to be printed. Sets it to the absolute value of the exponent subtracted by 1

        for(int i = 0; i < leadZeros; i++) { //Loops checks how many leading zero's are needed
            cout << '0'; //Prints out leading 0's
        }
        cout << '1'; //Prints out the leading value of 1
        for(int i = 0; i < 23; i++) { //For loop iterates through the size of the significant
            if(unsignedSig & 0x80000000) //If the signifcand bit is set
                cout << '1';
            else 
                cout << '0';
            unsignedSig = unsignedSig << 1; //Iterate through the significand
        }
        cout << endl;
    }
    return x;
}

/**
 * This function calls two seperate member functions in order to print out binary float numbers
 * 
 * This function calls printOutputLines and printBinaryOutput in order to read 32 bit hex values and format them into a specific output
 * format without using floating point containers. This is done in order to print out these hex values with precision.
 *
 * @param x Used as a pointer to store signed and unsigned 32 bit integar values be printed in the output 
 * 
 * @return A pointer to the unsigned 32 and signed 32 bit value
 *******************************************************************************************************************************************/
uint32_t printBinFloat(uint32_t x) {
    printOutputLines(x); //Calls member function printOutputLines to print out lines 1-4 of the output described in the assignment handout
    printBinaryOutput(x); //Calls member function printBinaryOutput to print out the 5th line, or the full value of the binary number

    return x;
}

/**
 * This function reads input files while calling member funciton printBinFloat
 * 
 * An unsigned 32-bit integar varibale is declared to be read through the input stream. Files with 32 bit hex values
 * can be extracted and stored in the variable x to be utilized in the printBinFloat function for output.
 *
 * @param x Used as a pointer to store signed and unsigned 32 bit integar values be printed in the output 
 * 
 * @return Returns false is the cin operation does not work due to a bad file being read in
 **************************************************************************************************************************/
int main() {
    uint32_t x; //Unsigned 32-bit integar variable

    while(cin >> hex >> x) { //The smart way to read in the input data according to class lecture ;)
        printBinFloat(x); //Calls member function printBinFloat
    }
    return 0; //Returns 0 if cin doesn't read in a valid file
}