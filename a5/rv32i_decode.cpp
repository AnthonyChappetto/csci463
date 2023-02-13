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
#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include <cassert>

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn) {
	uint32_t opcode = get_opcode(insn); //declarations for decode
	uint32_t funct7 = get_funct7(insn);    
	uint32_t funct3 = get_funct3(insn);
	int32_t imm_i = get_imm_i(insn);	

	switch(opcode) {
		default:	       return render_illegal_insn(insn);    //illegal
		case opcode_lui:   return render_lui(insn);
		case opcode_auipc: return render_auipc(insn);
		case opcode_rtype:
			switch(funct3) {
				default:  	  return render_illegal_insn(insn); //illegal
				case funct3_add:
					switch(funct7) {
						default:  	  return render_illegal_insn(insn);
						case funct7_add:  return render_rtype(insn, "add"); //addition and subtraction
						case funct7_sub:  return render_rtype(insn, "sub");
					} //r type cases
               	case funct3_sltu: return render_rtype(insn, "sltu");
				case funct3_xor:  return render_rtype(insn, "xor");     
				case funct3_sll:  return render_rtype(insn, "sll");
				case funct3_slt:  return render_rtype(insn, "slt");
			 	case funct3_srx:
					switch(funct7) {
						default:         return render_illegal_insn(insn);
                    	case funct7_sra: return render_rtype(insn, "sra");    
						case funct7_srl: return render_rtype(insn, "srl");
					}
                case funct3_and:  return render_rtype(insn, "and");    
				case funct3_or:   return render_rtype(insn, "or");
			}
		case opcode_stype:  //instructions to be executed if opcode matches stype
			switch(funct3) {
				default: 	return render_illegal_insn(insn);   //illegal
                case funct3_sh: return render_stype(insn, "sh");
				case funct3_sw: return render_stype(insn, "sw");
				case funct3_sb: return render_stype(insn, "sb");
			}
		case opcode_alu_imm:
			switch(funct3) {    //instructions to be executed that match itype opcodes
				default:          return render_illegal_insn(insn); //illegal
				case funct3_sll:  return render_itype_alu(insn, "slli", imm_i);
                case funct3_slt:  return render_itype_alu(insn, "slti", imm_i);
                case funct3_or:   return render_itype_alu(insn, "ori", imm_i);
				case funct3_and:  return render_itype_alu(insn, "andi", imm_i);
				case funct3_sltu: return render_itype_alu(insn, "sltiu", imm_i);
				case funct3_xor:  return render_itype_alu(insn, "xori", imm_i);
				case funct3_add:  return render_itype_alu(insn, "addi", imm_i);
			  	case funct3_srx:
					switch(funct7) {
						default:         return render_illegal_insn(insn);  //default illegal
						case funct7_sra: return render_itype_alu(insn, "srai", imm_i%XLEN);
                        case funct7_srl: return render_itype_alu(insn, "srli", imm_i);
					}					
			}
		case opcode_load_imm:
			switch(funct3) { //instructions to be executed that match itype opcodes
				default:	 return render_illegal_insn(insn);
				case funct3_lh:  return render_itype_load(insn, "lh");
				case funct3_lw:  return render_itype_load(insn, "lw");
				case funct3_lbu: return render_itype_load(insn, "lbu");
				case funct3_lhu: return render_itype_load(insn, "lhu");
                case funct3_lb:  return render_itype_load(insn, "lb");
			}
		case opcode_btype:  //instructions to be executed that match btype opcode
			switch(funct3) {
				default:	  return render_illegal_insn(insn);
				case funct3_bge:  return render_btype(addr, insn, "bge");
				case funct3_bltu: return render_btype(addr, insn, "bltu");
                case funct3_beq:  return render_btype(addr, insn, "beq");
				case funct3_bne:  return render_btype(addr, insn, "bne");
				case funct3_blt:  return render_btype(addr, insn, "blt");
				case funct3_bgeu: return render_btype(addr, insn, "bgeu");
			}
		case opcode_jal:  return render_jal(addr, insn);
		case opcode_jalr: return render_jalr(insn);	
		case opcode_system: //executes instyructions relating to csrrx
			switch(insn) {
				default:  
					switch(funct3) {
						default: 	     return render_illegal_insn(insn);
                        case funct3_csrrsi:  return render_csrrxi(insn, "csrrsi");
						case funct3_csrrci:  return render_csrrxi(insn, "csrrci");   
						case funct3_csrrw:   return render_csrrx(insn, "csrrw");
						case funct3_csrrwi:  return render_csrrxi(insn, "csrrwi");
        				case funct3_csrrs:   return render_csrrx(insn, "csrrs");
						case funct3_csrrc:   return render_csrrx(insn, "csrrc");
					}
				case insn_ecall:  return render_ecall(insn);
				case insn_ebreak: return render_ebreak(insn);
			}
	}
    assert (0 && "unrecognized opcode"); //impossible (SHOULD NEVER GET HERE GRRRRRR)
}

uint32_t rv32i_decode::get_opcode(uint32_t insn) {
	return (insn &0x0000007f); //return last 7 bits
}

uint32_t rv32i_decode::get_rd(uint32_t insn) {
    return ((insn &0x00000f80) >> 7); //gets bits from range 7-11
}

uint32_t rv32i_decode::get_funct3(uint32_t insn) {
	return ((insn &0x00007000) >> 12); //gets bits from range 12-24
}

uint32_t rv32i_decode::get_rs1(uint32_t insn) {
    return ((insn &0x000f8000) >> 15); //gets bits from range 15-19
}

uint32_t rv32i_decode::get_rs2(uint32_t insn) {
    return ((insn &0x01f00000) >> 20); //gets bits from range 20-24
}

uint32_t rv32i_decode::get_funct7(uint32_t insn) {
    return ((insn &0xfe000000) >> 25); //gets bits from range 25-31
}

int32_t rv32i_decode::get_imm_i(uint32_t insn) {
    int32_t imm_i = (insn &0xfff00000) >> (20 - 0);     //right shift selected bits from 20-0

    if (insn &0x80000000)    //if most significand bit
        imm_i |= 0xfffff000; //sign extend

    return imm_i;
}

int32_t rv32i_decode::get_imm_u(uint32_t insn) {
    return (insn & 0xfffff000); //Sets bits 0-11 as 0
}

int32_t rv32i_decode::get_imm_b(uint32_t insn) {
    int32_t imm_b = (insn &0x80000000) >> (31 - 12);
    imm_b |= (insn &0x7e000000) >> (25 - 5); //right shift bits 25-5
    imm_b |= (insn &0x00000f00) >> (8 - 1); //right shift bits 8-1
    imm_b |= (insn &0x00000080) << (11 - 7); //left shift bits 11-7

    if (insn &0x80000000)    //if most significand bit
        imm_b |= 0xfffff000; //sign extend

    return imm_b;
}

int32_t rv32i_decode::get_imm_s(uint32_t insn) {
    int32_t imm_s;
    imm_s = (insn &0xfe000000) >> 20; //right shift bits 20
    imm_s |= (insn &0x00000f80) >> 7; //right shift bits 7-0

    if (insn &0x80000000) //if most significand bit
        imm_s |= 0xfffff000; //sign extend
    
    return imm_s;
}

int32_t rv32i_decode::get_imm_j(uint32_t insn) {
    int32_t imm_j = (insn &0x000ff000);
    imm_j |= (insn &0x00100000) >> (20-11); //right shift bits 10-11
    imm_j |= (insn &0x7fe00000) >> (30-10); //right shift bits 30-10
    imm_j |= (insn &0x80000000) >> (31-20); //right shift bits 31-20

    if (insn &0x80000000) //if msb
        imm_j |= 0xfff00000; //sign extend

    return imm_j;
}

std::string rv32i_decode::render_illegal_insn(uint32_t insn) {
    (void)insn; //casts insn to a void
    return "ERROR: UNIMPLEMENTED INSTRUCTION"; //Returns error message
}

std::string rv32i_decode::render_lui(uint32_t insn) {
    int32_t imm_u = get_imm_u(insn); //declares rd and imm_u
    uint32_t rd = get_rd(insn);

    ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd) << "," << hex::to_hex0x20(imm_u); //sets string lui to decode version and covert to hex0x20
    return os.str();
}

std::string rv32i_decode::render_auipc(uint32_t insn) {
    int32_t imm_u = get_imm_u(insn); //declares imm_u and rd
    uint32_t rd = get_rd(insn);

    ostringstream os; //sets auipc to decoded version and converts to hex0x20
    os << render_mnemonic("auipc") << render_reg(rd) << "," << hex::to_hex0x20(imm_u);
    return os.str();
}

std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn) {
    int32_t pcrel_21 = get_imm_j(insn)+addr;    //declares pcrel_21 and rd
    uint32_t rd = get_rd(insn);

    ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << ",0x" << //sets string jal to decoded version
        hex::to_hex32(pcrel_21); //coverts string to hex0x32
    return os.str();
}

std::string rv32i_decode::render_jalr(uint32_t insn) {
    uint32_t rd = get_rd(insn); //declares rd, imm_i, and rs1
    int32_t imm_i = get_imm_i(insn);    
    uint32_t rs1 = get_rs1(insn);

    ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," //sets string jalr
       << render_base_disp(rs1, imm_i); //passes render_base_disp with rs1 and imm_i
    return os.str();
}

std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char* mnemonic) {
    int32_t pcrel_13 = get_imm_b(insn) + addr;    //declares pcrel_13, rs1, and rs2
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << ",x" << rs2 << ",0x" //sets string to mnemonic and rs1
       << hex::to_hex32(pcrel_13); //converts string to hex0x32
    return os.str();
}

std::string rv32i_decode::render_itype_load(uint32_t insn, const char* mnemonic) {
    uint32_t rd = get_rd(insn);     //declares rd, rs1, and imm_i
    uint32_t rs1 = get_rs1(insn);   
    int32_t imm_i = get_imm_i(insn);

    ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," //sets string to mnemonic and rd
       << render_base_disp(rs1, imm_i); //passes render_base_disp with rs1 and imm_i
    return os.str();
}

std::string rv32i_decode::render_stype(uint32_t insn, const char* mnemonic) {
    int32_t imm_s=get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    ostringstream os;
    os << render_mnemonic(mnemonic) //sets string to mnemonic and rs2
    << render_reg(rs2) << "," 
    << render_base_disp(rs1, imm_s); //passes render_base_Disp with rs1, and imm_s
    return os.str();
}

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char* mnemonic, int32_t imm_i) {
    uint32_t rd = get_rd(insn);     //declares rd and rs1
    uint32_t rs1 = get_rs1(insn);

    ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) //sets string to mnemonic and rd
    << ",x" << rs1 << "," << imm_i; //passes rs1 and imm_i
    return os.str();
}

std::string rv32i_decode::render_rtype(uint32_t insn, const char* mnemonic) {
    uint32_t rd = get_rd(insn);     //declares rd, rs1, and rs2
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << ",x" //sets strings to menmonic and rd
       << rs1 << ",x" << rs2;                                 //passes rs1 and rs2
    return os.str();
}

std::string rv32i_decode::render_ecall(uint32_t insn) {
    (void)insn; //cats insn as a void
    ostringstream os;
    os << "ecall";
    return os.str();    //returns the ecall as a string
}

std::string rv32i_decode::render_ebreak(uint32_t insn) {
    (void)insn;         //casts insn as a void
    ostringstream os;
    os << "ebreak";
    return os.str();    //returns the ebreak as a string
}

std::string rv32i_decode::render_csrrx(uint32_t insn, const char* mnemonic) {
    uint32_t rd = get_rd(insn);     //declares rd, rs1, and csr
    uint32_t rs1 = get_rs1(insn);    
    int32_t csr = get_imm_i(insn);

    ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," //sets string to mnemonic and rd
       << hex::to_hex0x12(csr) << ",x" << rs1;               //passes csr as a hex0x12 and render_reg() as rs1
    return os.str();
}

std::string rv32i_decode::render_csrrxi(uint32_t insn, const char* mnemonic) {
    uint32_t rd = get_rd(insn);     //declares rd, csr, and zimm
    int32_t csr = get_imm_i(insn);
    uint32_t zimm = get_rs1(insn);

    ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," //sets mnemonic and rd
       << hex::to_hex0x12(csr) << "," << zimm;  //passes csr as a hex0x12
    return os.str();
}

std::string rv32i_decode::render_reg(int r) {
    ostringstream os;
    os << "x" << r;
    return os.str(); //returns r "x" as a str()
}

std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp) {
    ostringstream os;
    os << disp << "(" << render_reg(base) << ")"; //set string to register in parenthese
    return os.str();
}

std::string rv32i_decode::render_mnemonic(const std::string &m) {
    ostringstream os;
    os << setw(mnemonic_width) << setfill(' ') << left << m; //sets string with set width and to the left
    return os.str();
}