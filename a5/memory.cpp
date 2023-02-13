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
#include "memory.h"
#include "hex.h"

memory::memory(uint32_t siz) {
    
    siz = (siz + 15)&0xfffffff0; //round the length up, mod-16

    mem.resize(siz, 0xa5); //Use resize() member function and initialize every byte/element to 0xa5
}

memory::~memory() {
    mem.clear(); //Call clear() to clear the memory vector
}

bool memory::check_illegal(uint32_t i) const {
    if (!(i < mem.size())) { //If i is not less than get_size()
        cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << endl; //Display warning error
        return true;
    }

    return !(i < mem.size()); //Return false if the element is "illegal"
}

uint32_t memory::get_size() const {
    return mem.size(); //Calls size() to return the rounded up number of bytes in memory
}

uint8_t memory::get8(uint32_t addr) const{
    if (check_illegal(addr)) //Calls check_illegal
        return 0; //Return false if check_illegal comes back positive
    else
        return mem.at(addr); //Returns element of mem vector placed in addr
}

uint16_t memory::get16(uint32_t addr) const {
    uint16_t x;
    uint16_t y;
    x = get8(addr); //Calls get8()
    y = get8(addr + 1);

    uint32_t lilEndian;
    lilEndian = (y << 8) | x; //combines x and y into lilEndian
    return lilEndian;
}

uint32_t memory::get32(uint32_t addr) const {
    uint32_t x;
    uint32_t y;
    x = get16(addr); //calls get16()
    y = get16(addr + 2);

    uint32_t lilEndian;
    lilEndian = (y << 16) | x; //combines x and y into lilEndian
    return lilEndian;
}

int32_t memory::get8_sx(uint32_t addr) const {
    int32_t val = get8(addr); //set val to element grabbed from get8
    return val | (val&0x00000080 ? 0xffffff00 : 0); //Returns the value based on octal setup
}

int32_t memory::get16_sx(uint32_t addr) const {
    int32_t val = get16(addr); //set val to element grabbed from get16
    return val | (val&0x00008000 ? 0xffff0000 : 0); //Returns the value based on octal setup
}

int32_t memory::get32_sx(uint32_t addr) const {
    return get32(addr); //Returns get32 as addr as a param
}

void memory::set8(uint32_t addr, uint8_t val) {
    if(!check_illegal(addr)) //Ensures check_illegal works for addr element
        mem.at(addr) = val; //Places addr element of mem array into val
}

void memory::set16(uint32_t addr, uint16_t val) {
    uint8_t x; //Declares 2 uint8_t varibales
    uint8_t y;
    x = ((val &0xFF00) >> 8); //sets x to hold the first 2 bits of 4
    y = (val &0x00FF); //sets y to hold the last 2 bits of 4
    set8(addr+1, x); //Calls set8() to increase addr element and place x and y together in little endian order
    set8(addr, y);
}

void memory::set32(uint32_t addr, uint32_t val) {
    uint16_t x; //Declares 2 uint16_t variables
    uint16_t y;
    x=((val &0xFFFF0000) >> 16); //sets x to hold the first 4 bits of 8
    y=(val &0x0000FFFF); //sets y to hold the last 4 bits of 8
    set16(addr + 2, x); //Calls set16() twice to put x and y in little endian order and increase the addr
    set16(addr, y);
}

void memory::dump() const {
    char dumpArray[17]; //Creation of array to store mem
    dumpArray[16] = '\0'; //Label index 16 to \0

    unsigned z = 0; 
    while(z<mem.size()) { //Loop pushes memory back into ch
        uint8_t ch = get8(z);
        ch = isprint(ch) ? ch : '.';

        dumpArray[z%16] = ch; //Set the array to ch

        if((z%16) == 0)
            cout << hex::to_hex32(z) << ": "; //converts to_hex32

        cout << hex::to_hex8(get8(z)) 
        << " "; //prints out through to_hex8
    
        if((z % 16) == 15) {
            cout << "*" << dumpArray << "*" 
            << endl; 
        }
        else if ((z % 16) == 7) { //prints out spacing via mod 16 division
            cout << " "; 
        }
        z++;
  }
}

bool memory::load_file(const std::string &fname) {
    ifstream infile(fname, std::ios::in|std::ios::binary); //Declaring file ifstream from course page
    if (infile.is_open()) { //opens the file
        uint8_t i;
        infile >> noskipws; //set noskipws before reading as instructed in handout
        for (uint32_t addr = 0; infile >> i; ++addr) { 
            if (check_illegal(addr)) {
                cerr << "Program too big." << endl; //Error message if check_iillegal is verified
                return false; 
                }
            set8(addr, i); //incremenet the count and value through set8()
            }
        return true; 
        }
    else {
        cerr << "Can't open file '" << fname << "' for reading." << endl; //Prints out error message if file cannot be open
        return false;
        }
}