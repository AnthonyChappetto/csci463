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
#include "rv32i_hart.h"
#include <iomanip>
#include <iostream>
#include <cstdint>
#include <string>

void rv32i_hart::tick(const std::string &hdr) {
	uint32_t insn;

	if(!halt) { //if halt does not occur
		insn_counter++; //update insn_counter
		if(show_registers)
            dump(hdr);  //dump hdr is show_registers occur

		insn=mem.get32(pc); //set insn to mem.get32()

		if(show_instructions) {
			cout << hdr << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << "  ";    //hex0x32 conversion
			exec(insn, &cout);	
		}
		else 
            exec(insn, nullptr);    //or exec instr with nullptr
	}
	else 
        return;
}

void rv32i_hart::dump(const std::string &hdr) const {
	regs.dump(hdr); //dump regs

	cout << " pc " << hex::to_hex32(pc) << endl;
}

void rv32i_hart::reset() {
    pc = 0; //sets program counter to 0
    regs.reset();   //reset the regs
    halt_reason = "none";     //set halt_reason
    halt = false;
    insn_counter = 0;
}

void rv32i_hart::exec(uint32_t insn, std::ostream* pos) {
	uint32_t opcode = get_opcode(insn); //declarations for opcode, funct3, and funct7
	uint32_t funct3 = get_funct3(insn);
	uint32_t funct7 = get_funct7(insn);

	switch(opcode) {
		default:	   exec_illegal_insn(insn, pos); return;    //ilegal
        case opcode_auipc: exec_auipc(insn, pos); return;   
		case opcode_lui:   exec_lui(insn, pos); return;
		case opcode_rtype:
			switch(funct3) {    //r type cases
				default:  	  exec_illegal_insn(insn, pos); return; //illegal
				case funct3_add:
					switch(funct7) {
						default:  	  exec_illegal_insn(insn, pos); return; //illegal
						case funct7_add:  return exec_add(insn, pos); return;   //addition and subtraction
						case funct7_sub:  return exec_sub(insn, pos); return;
					}
                case funct3_sltu: exec_sltu(insn, pos); return; //funct3 rtypes
				case funct3_xor:  exec_xor(insn, pos); return;    
			    case funct3_sll:  exec_sll(insn, pos); return;
				case funct3_slt:  exec_slt(insn, pos); return;
			 	case funct3_srx:
					switch(funct7) {
						default:         exec_illegal_insn(insn, pos); return;
						case funct7_sra: exec_sra(insn, pos); return;
                        case funct7_srl: exec_srl(insn, pos); return;
					}
				case funct3_and:  exec_and(insn, pos); return;  //and & or
                case funct3_or:   exec_or(insn, pos); return;
			}
		case opcode_stype:  //s type instructions
			switch(funct3) {
				default: 	exec_illegal_insn(insn, pos); return;
				case funct3_sw: exec_sw(insn, pos); return;
                case funct3_sb: exec_sb(insn, pos); return;
				case funct3_sh: exec_sh(insn, pos); return;
			}
		case opcode_alu_imm:    //itype opcodes
			switch(funct3) {
				default:          exec_illegal_insn(insn, pos); return; //illegal
				case funct3_sltu: exec_sltiu(insn, pos); return;
                case funct3_slt:  exec_slti(insn, pos); return;
				case funct3_or:   exec_ori(insn, pos); return;
				case funct3_and:  exec_andi(insn, pos); return;
				case funct3_xor:  exec_xori(insn, pos); return;
            	case funct3_sll:  exec_slli(insn, pos); return;
				case funct3_add:  exec_addi(insn, pos); return;
			  	case funct3_srx:
					switch(funct7) {    //funct7 
						default:         exec_illegal_insn(insn, pos); return;  //illegal
                        case funct7_sra: exec_srai(insn, pos); return;
						case funct7_srl: exec_srli(insn, pos); return;
					}					
			}
		case opcode_load_imm:   //itype opcodes
			switch(funct3) {
				default:	 exec_illegal_insn(insn, pos); return;  //illegal
				case funct3_lb:  exec_lb(insn, pos); return;
				case funct3_lbu: exec_lbu(insn, pos); return;
                case funct3_lh:  exec_lh(insn, pos); return;
				case funct3_lw:  exec_lw(insn, pos); return;
				case funct3_lhu: exec_lhu(insn, pos); return;
			}
		case opcode_btype:  //btype opcodes
			switch(funct3) {
				default:	  exec_illegal_insn(insn, pos); return; //illegal
                case funct3_bltu: exec_bltu(insn, pos); return;
				case funct3_bgeu: exec_bgeu(insn, pos); return;
				case funct3_bge:  exec_bge(insn, pos); return;
                case funct3_beq:  exec_beq(insn, pos); return;
				case funct3_bne:  exec_bne(insn, pos); return;
				case funct3_blt:  exec_blt(insn, pos); return;
			}
		case opcode_jal:  exec_jal(insn, pos); return;
		case opcode_jalr: exec_jalr(insn, pos); return;
		case opcode_system: //csrrx instr
			switch(insn) {
				default:  
					switch(funct3)
					{
						default: 	     exec_illegal_insn(insn, pos); return;  //illegal
						case funct3_csrrs:   exec_csrrs(insn, pos); return;
					}
				case insn_ebreak: exec_ebreak(insn, pos); return;   //calls ebreak
			}
	}			
}

void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos) {
	(void)insn; //casts insn to void
	
	if(pos) 
        *pos << render_illegal_insn(insn);

	halt = true;
	halt_reason = "Illegal instruction";    //set halt to true for Illegal Instr
}

void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos) {
    int32_t rs1 = get_rs1(insn); //declares rs1, rs2, and rd
    int32_t rs2 = get_rs2(insn);
    int32_t rd = get_rd(insn);
    int32_t val = regs.get(rs1)+regs.get(rs2);

    if(pos) {
        string s = render_rtype(insn, "add"); //declares string for add instr
        *pos << setw(instruction_width) << setfill(' ') << left << s; //formatting for pos
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " <<  //sets string add to decoded version
             hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl; //converts to hex0x32
    }
    regs.set(rd, val); //sets regs to rd and val
    pc += 4; //incr pc counter
}

void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos) {
    int32_t imm_i = get_imm_i(insn);  //declares imm_i, rd, rs1, and sum
    int32_t rd = get_rd(insn);      
    int32_t rs1 = get_rs1(insn);
    uint32_t sum = regs.get(rs1) + imm_i;

    if(pos) {
        string s=render_itype_alu(insn, "addi", imm_i); //declares string for addi instr
        *pos << setw(instruction_width) << setfill(' ') << left << s; //formatting for pos
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " <<  //sets string addi to decoded version
        hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(sum) << endl; //converts to hex0x32
    }
    regs.set(rd, sum); //sets regs to rd and sum
    pc+=4; //incr pc counter
}

void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos) {
    int32_t rs1=get_rs1(insn); //declares variables for rs1, rs2, rd, and val
    int32_t rs2=get_rs2(insn);
    int32_t rd=get_rd(insn);    
    int32_t val = regs.get(rs1)&regs.get(rs2); 

    if(pos) {
        string s=render_rtype(insn, "and"); //declares string for and instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //formatting for pos
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " <<  //sets string and to decoded version
        hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl; //converts to hex0x32
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc += 4;    //incr pc counter
}

void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos) {
    int32_t rd=get_rd(insn);        //declares variables for rd, rs1, imm_i, and val
    int32_t rs1=get_rs1(insn);
    int32_t imm_i=get_imm_i(insn);
    int32_t val=(regs.get(rs1) &imm_i);

    if(pos) {
        string s = render_itype_alu(insn, "andi", imm_i);       //declares string for andi instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;       //formatting for pos
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " <<  //sets string andi to decoded version
             hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;   //converts to hex0x32
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc+=4;  //incr pc counter
}

void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos) {
	int32_t imm_u = get_imm_u(insn);    //declares variables for imm_u, rd, and val  
	uint32_t rd = get_rd(insn);
	int32_t val = imm_u + pc;

	if(pos) {
		string s=render_auipc(insn);    //declares string for auipc instr
		*pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_u) << " = " << //sets string auipc to decoded version
        hex::to_hex0x32(val) << endl;   //converts to hex0x32
	}
	regs.set(rd, val);  //sets regs to rd and val
	pc+=4;  //incr pc counter
}

void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos) {
    int32_t t_addr;             //variable declarations for t_addr, rs1, rs2, and imm_b
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);    

    if(regs.get(rs1) != regs.get(rs2)) { //if retrieved reg 1 does not equal reg 2
        t_addr = pc + 4;        //set t_addr to program counter + 4
    }
   	else {
      	t_addr = pc + imm_b;    //else set t_addr to program counter + imm_b
    }

    if(pos) {
        string s=render_btype(pc, insn, "beq"); //declares string for beq instruction
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == " << hex::to_hex0x32(regs.get(rs2)) << " ? " << //sets string beq to decode version
        hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << endl;    ///hex0x32 conversion
    }
    pc=t_addr;  
}

void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos) {
    int32_t imm_b = get_imm_b(insn);    //variable declarations for imm_b, rs1, rs2, and t_addr
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t t_addr;

    if(regs.get(rs1) >= regs.get(rs2)) {    //if reg rs1 is greater than or equal to reg2
      	t_addr = pc + imm_b;    //set t_addr to program counter + imm_b
    }
    else {
        t_addr = pc + 4; //else set t_addr to pc + 4
    }

    if(pos) {
        string s = render_btype(pc, insn, "bge");       //variable declaration for bge string
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos  formatting
	    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << " ? " <<  //sets bge string to decoded version
             hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << endl;   //hex0x32 conversion
    }
    pc=t_addr;
}

void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos) {
    int32_t imm_b=get_imm_b(insn);    //variable declarations for imm_b, rs1, rs2, and t_addr
    int32_t rs1=get_rs1(insn);
    int32_t rs2=get_rs2(insn);
    int32_t t_addr;

    if((uint32_t)regs.get(rs1) >= (uint32_t)regs.get(rs2)) { //if rs1 is greater than or equal to rs2
      	t_addr = pc + imm_b;    //set t_addr to program counter + imm_b
    }
    else {
        t_addr = pc + 4;    //else set t_addr to program counter + 4
    }

    if(pos) {
        string s=render_btype(pc, insn, "bgeu");    //declares string for bgeu instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << //set bgeu string to decoded version
         hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << endl; //hex0x32 conversion
    }
    pc=t_addr;
}

void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos) {
    int32_t rs1 = get_rs1(insn);        //variable declarations for rs1, rs2, imm_b, and t_addr
    int32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    int32_t t_addr;

    if(regs.get(rs1)<regs.get(rs2)) {   //if reg rs1 < reg rs2
      	t_addr=pc + imm_b;  //set t_addr to program counter + imm_b
    }
    else {
        t_addr=pc + 4;  //else set t_addr to program counter + 4
    }
    
    if(pos) {
        string s = render_btype(pc, insn, "blt");   //declares string for blt instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;       //pos formatting
	    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << " ? " <<   //set blt to decode version
        hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << endl; //hex 0x32 conversion
    }
    pc=t_addr;  
}

void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos) {
    int32_t t_addr;    //variable declarations to t_addr, rs1, rs2, and imm_b
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);

    if((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2)) //if rs1 is less than rs2 
        t_addr=pc + imm_b;  //set t_addr to program counter + imm_b
    else
        t_addr=pc + 4;  //else set t_addr to program counter + 4
    
    if(pos) {
        string s = render_btype(pc, insn, "bltu");  //sets string for bltu instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << //sets bltu string to decoded version
        hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << endl;    //hex 0x32 conversion
    }
    pc=t_addr;
}

void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos) {
    int32_t imm_b = get_imm_b(insn);    //declares variables for imm_b, rs1, rs2, and t_addr
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    int32_t t_addr;

    if(regs.get(rs1) == regs.get(rs2)) //if rs1 and rs2 regs are equal
        t_addr = pc + 4;    //set t_addr to program counter + 4
    else
        t_addr = pc + imm_b;    //else set t_addr to program counter + imm_b
    
    if(pos) {
	    string s = render_btype(pc, insn, "bne");       //string set for bne instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;       //pos formatting
	    *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != " << hex::to_hex0x32(regs.get(rs2)) << " ? " << //sets bne string to decoded version
             hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(t_addr) << endl;  //hex 0x32 conversion
    }
    pc=t_addr; 
}

void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos) {
	int32_t csr = get_imm_i(insn) &0x00000fff;    //variable declarations for csr, rd, and rs1
	uint32_t rd=get_rd(insn);
	uint32_t rs1=get_rs1(insn);

	if(rs1 != 0 || csr != 0xf14) { //if rs1 is not equal to 0 or csr is not equal to 0xf14
		halt =true;
		halt_reason ="Illegal CSR in CRSS instruction"; //set halt to true and print out halt_reason
	}
	
	if(pos) {
		string s = render_csrrx(insn, "csrrs");     //set string for csrrs instr
		*pos << setw(instruction_width) << setfill(' ') << left << s;   //formating for pos
		*pos << "// " << render_reg(rd) << " = " << dec << mhartid << endl; //set csrrs to decoded version
	}

	if(halt) 
        return;
    else {
		regs.set(rd, mhartid); //set the regs to rd, mhartid
		pc+=4;  //incr pc by 4
	}
}

void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos) {
	if(pos) {
		string s = render_ebreak(insn); //set string for ebreak instr
		*pos << setw(instruction_width) << setfill(' ') << left << s; //formatting for pos
		*pos << "// HALT" << endl;  //print out HALT
	}
	halt=true;
	halt_reason="EBREAK instruction"; //print out halt_reason and halt prgm
}

void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos) {
	int32_t imm_j = get_imm_j(insn);      //variable dec for imm_j, rd, and val
	uint32_t rd = get_rd(insn);
	int32_t val = pc + imm_j;      

	if(pos) {
		string s = render_jal(pc, insn);    //set string to jal instr
		*pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) << ",  pc = " << //set jal string to decoded version
             hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(val) << endl; //hex 0x32 conversion
	}
	regs.set(rd, pc + 4);   //set regs to rd and pc + 4
	pc=val; 
}

void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos) {
	uint32_t rs1 = get_rs1(insn);    //variable declarations for rs1, rd, imm_i and val
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t val = (regs.get(rs1) + imm_i) & 0xfffffffe;

	if(pos) {
		string s = render_jalr(insn);   //set string to jalr instr
		*pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) << ",  pc = (" << hex::to_hex0x32(imm_i) << " + " << //set jalr string to decoded version
             hex::to_hex0x32(regs.get(rs1)) << ") & 0xfffffffe = " << hex::to_hex0x32(val) << endl; //hex 0x32 conversion
	}
	regs.set(rd, pc + 4);   //set regs to rd and pc + 4
	pc=val;
}

void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos) {
    int32_t number =0x80;    //declare variables for number, rd, rs1, imm_i, t_addr, and val
    int32_t rd = get_rd(insn);    
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t t_addr = regs.get(rs1) + imm_i;    
    uint32_t val = mem.get8(t_addr);   

    val=0xff &val;

    if(number &val)     
        val+=0xffffff00;    //set the val

    if(pos)
    {
        string s = render_itype_load(insn, "lb");   //set string for lb instr
        *pos << setw(instruction_width) << setfill(' ') << left << s; //pos formatting
        *pos << "// " << render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << //set lb string to decoded version
             hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;     //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs rd and val
    pc+=4;  //incr program counter by 4
}

void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos) {
    int32_t rs1 = get_rs1(insn);        //variable dec for rs1, rd, imm_i, t_addr, and val
    int32_t rd = get_rd(insn);          
    int32_t imm_i = get_imm_i(insn);
    uint32_t t_addr = regs.get(rs1)+imm_i;
    uint32_t val = mem.get8(t_addr) &0x000000ff;

    if(pos) {
        string s=render_itype_load(insn, "lbu");    //set string for lbu instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << //set lbu string to decoded version
             hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;  //hex0x32 conversion
    }
    regs.set(rd, val); //set regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos) {
    int32_t imm_i=get_imm_i(insn);    //variable declarations for imm_i, rs1, rd, t_addr, and val
    int32_t rs1=get_rs1(insn);
    int32_t rd=get_rd(insn);
    uint32_t t_addr=regs.get(rs1)+imm_i;
    uint32_t val=mem.get16(t_addr);

    val = 0xffff &val; //sets val and num
    int32_t num=0x8000;

    if(num &val)
        val += 0xffff0000;  //incr val

    if(pos) {
        string s = render_itype_load(insn, "lh");     //lh string declaration
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " <<   //converts lh to decoded version
             hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl; //hex 0x32 conversion
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos) {
    int32_t imm_i = get_imm_i(insn);    //variable declarations for imm_i, rs1, rd, t_addr, and val
    int32_t rs1 = get_rs1(insn);
    int32_t rd = get_rd(insn);
    uint32_t t_addr = regs.get(rs1)+imm_i;
    uint32_t val = mem.get16(t_addr)&0x0000ffff;

    if(pos) {
        string s=render_itype_load(insn, "lhu");    //lhu string declaration
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << //sets lhu string to decoded version
             hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl; //hex 0x32 conversions
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos) {
   	int32_t imm_u = get_imm_u(insn);    //sets variables imm_u and rd
   	uint32_t rd = get_rd(insn);

   	if(pos) {
		string s = render_lui(insn);    //sets string to lui instr
		*pos << setw(instruction_width) << setfill(' ') << left << s; //pos formatting
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u) << endl; //hex0x32 conversion and decoding
    }
	regs.set(rd, imm_u);    //sets regs to rd and imm_u
	pc+=4;   //incr pc by 4
}

void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos) {
    int32_t rd=get_rd(insn);        //variable declarations for rd, rs1, imm_i, t_addr, and val
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t t_addr=regs.get(rs1)+imm_i;
    uint32_t val = mem.get32(t_addr);

    if(pos) {
        string s = render_itype_load(insn, "lw");   //string declaration for lw
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << //lw decoding
             hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl; //hex0x32 conversion
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos) {
    int32_t rs1 = get_rs1(insn);    //variable declarations for rs1, rs2, rd, and val
    int32_t rs2 = get_rs2(insn);
    int32_t rd = get_rd(insn);
    int32_t val = regs.get(rs2) | regs.get(rs1); 

    if(pos) {
        string s=render_rtype(insn, "or");  //sets string for or instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << //decoding or instr
             hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl; //hex0x32 conversion
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos) {
    int32_t imm_i=get_imm_i(insn);    //variable dec for imm_i, rd, rs1, and val
    int32_t rd=get_rd(insn);
    int32_t rs1=get_rs1(insn);
    int32_t val=regs.get(rs1) | imm_i;

    if(pos) {
       	string s = render_itype_alu(insn, "ori", imm_i);    //sets string for ori instr
        *pos << std::setw(instruction_width) << setfill(' ') << left << s;  //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " <<  //ori decoding
             hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;   //hex0x32 conversion
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos) {
    int32_t imm_s = get_imm_s(insn);    //sets variables for imm_s, rs1, rs2, t_addr, and val
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    uint32_t t_addr = regs.get(rs1)+imm_s;
    uint32_t val = regs.get(rs2) &0x000000ff;

    if(pos) {
        string s = render_stype(insn, "sb");    //sets string for sb instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << //sb decodding
             hex::to_hex0x32(val) << endl;  //hex0x32 conversion
    }
    mem.set8(t_addr, val);  //sets regs to t_addr and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos) {
    int32_t imm_s=get_imm_s(insn);     //declares variables for imm_s, rs1, rs2, t_addr, and val
    int32_t rs1=get_rs1(insn);
    int32_t rs2=get_rs2(insn);
    uint32_t t_addr=regs.get(rs1)+imm_s;
    uint32_t val=regs.get(rs2) &0x0000ffff;

    if(pos) {
        string s = render_stype(insn, "sh");    //sets string for sh instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << //sh decodding
             hex::to_hex0x32(val) << endl;  //hex0x32 conversion
    }
    mem.set16(t_addr, val); //sets mem to t_addr and val
    pc+=4; //incr pc by 4
}

void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos) {
    int32_t rs1 = get_rs1(insn);        //variable declarations for rs1, rs2, rd, sll, and val
    int32_t rs2 = get_rs2(insn);
    int32_t rd = get_rd(insn);    
    int32_t sll = regs.get(rs2) &0x01f;
    int32_t val = regs.get(rs1) << sll;

    if(pos) {
        string s = render_rtype(insn, "sll");   //sets string for sll instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " << //sll decodding
		     dec << sll << " = " << hex::to_hex0x32(val) << endl; //hex0x32 conversion
    }
    regs.set(rd, val); //sets regs to rd and val
    pc+=4; //pc incr by 4
}

void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos) {
    int32_t rd=get_rd(insn);    //variable declarations for rd, rs1, xlenshift, and val
    int32_t rs1=get_rs1(insn);
    int32_t xlenshift=get_imm_i(insn)%XLEN;
    int32_t val = regs.get(rs1) << xlenshift;

    if(pos) {
        string s = render_itype_alu(insn, "slli", get_imm_i(insn)); //string declaration for slli instr
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " <<     //slli decodding 
		     dec << xlenshift << " = " << hex::to_hex0x32(val) << endl;     //hex0x32 formatting
    }
    regs.set(rd, val);  //set regs for rd and val
    pc+=4;  //update pc by 4
}

void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos) {
    uint32_t rd = get_rd(insn);     //variable declarations for rd, rs1, rs2, and val
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

    if(pos) {
        string s = render_rtype(insn, "slt");       //string declaration for slt
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << //slt decodding
             hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;  //hex0x32 conversion
    }
    regs.set(rd, val);  //setting regs to rd and val
    pc+=4;  //updating pc by 4
}

void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos) {
    int32_t rd = get_rd(insn);      //variable dec for rd, rs1, imm_i, and val
    int32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t val = (regs.get(rs1) < imm_i) ? 1 : 0;

    if(pos) {
	    string s = render_itype_alu(insn, "slti", imm_i);       //slti string 
	    *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
	    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " <<     //decodding slti
             dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;    //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos) {
    int32_t rd=get_rd(insn);        //variable dec for rd, rs1, rs2, and val
    int32_t rs1=get_rs1(insn);
    int32_t rs2=get_rs2(insn);
    int32_t val=((uint32_t)regs.get(rs2) > (uint32_t)regs.get(rs1)) ? 1 : 0;

    if(pos) {
        string s = render_rtype(insn, "sltu");  //sltu string
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " <<  //sltu decodding
             hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;  //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos) {
    int32_t rd=get_rd(insn);            //variable dec for rd, rs1, imm_i, and val
    int32_t rs1=get_rs1(insn);
    int32_t imm_i=get_imm_i(insn);
    int32_t val=((uint32_t)imm_i) > (uint32_t)regs.get(rs1) ? 1 : 0;

    if(pos) {
        string s = render_itype_alu(insn, "sltiu", imm_i);      //sltiu string
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " <<    //sltiu decodding
		     dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;    //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos) {
    int32_t rs1=get_rs1(insn);  //variable dec for rs1, rs2, rd, shift, and val
    int32_t rs2=get_rs2(insn);
    int32_t rd=get_rd(insn);
    int32_t shift=regs.get(rs2) &0x0000001f;
    int32_t val=regs.get(rs1) >> shift; 

    if(pos) {
        string s = render_rtype(insn, "sra");   //sra string
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << //sra decodding
		     dec << shift << " = " << hex::to_hex0x32(val) << endl; //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs to rd and val
    pc+=4;  //update pc by 4
}

void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos) {
    int32_t rs1 = get_rs1(insn);    //variable dec for rs1, rd, xlenshift, and val
    int32_t rd = get_rd(insn);
    int32_t xlenshift = get_imm_i(insn)%XLEN;
    int32_t val = regs.get(rs1) >> xlenshift;

    if(pos) {
        string s = render_itype_alu(insn, "srai", (int32_t)get_imm_i(insn)%XLEN);   //string srai
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << //srai decodding
		     dec << xlenshift << " = " << hex::to_hex0x32(val) << endl; //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos) {
    int32_t rs1 =get_rs1(insn);     //variable dec for rs1, rs2, rd, shift, and val
    int32_t rs2 =get_rs2(insn);
    int32_t rd =get_rd(insn);
    int32_t shift =regs.get(rs2) &0x0000001f;
    int32_t val = (uint32_t)regs.get(rs1) >> shift; 

    if(pos) {
        string s = render_rtype(insn, "srl");       //srl string
        *pos << setw(instruction_width) << setfill(' ') << left << s;       //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << //srl deocdding
		     dec << shift << " = " << hex::to_hex0x32(val) << endl; //hex0x32 conversion
    }
    regs.set(rd, val); //set regs to rd and val
    pc+=4; //incr pc by 4
}

void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos) {
    int32_t rs1= get_rs1(insn);    //variable dec for rs1, rd, xlenshift, and val
    int32_t rd= get_rd(insn);
    int32_t xlenshift =get_imm_i(insn)%XLEN;
    int32_t val = (uint32_t)regs.get(rs1) >> xlenshift;

    if(pos) {
        string s=render_itype_alu(insn, "srli", get_imm_i(insn));   //srli string
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " <<     //srli decodding
		dec << xlenshift << " = " << hex::to_hex0x32(val) << endl; //hexox32 conversion
    }
    regs.set(rd, val);
    pc+=4;
}

void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos) {
    int32_t rs1=get_rs1(insn);  //variable dec for rs1, rs2, rd, and val
    int32_t rs2=get_rs2(insn);
    int32_t rd=get_rd(insn);    
    int32_t val = regs.get(rs1)-regs.get(rs2);

    if(pos) {
        string s=render_rtype(insn, "sub"); //string sub
        *pos << setw(instruction_width) << setfill(' ') << left << s;       //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " <<  //sub decodding
             hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;   //hex0x32 conversion
    }
    regs.set(rd, val);  //sets regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos) {
    int32_t rs1=get_rs1(insn);  //variable dex for rs1, rs2, imm_s, t_addr. amd val
    int32_t rs2=get_rs2(insn);
    int32_t imm_s=get_imm_s(insn);
    uint32_t t_addr=regs.get(rs1)+imm_s;
    uint32_t val=regs.get(rs2);

    if(pos) {
        string s = render_stype(insn, "sw");    //string dec for sw
        *pos << setw(instruction_width) << setfill(' ') << left << s;   //pos formatting
        *pos << "// m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " <<     //sw decodding
             hex::to_hex0x32(val) << endl;  //hex0x32 conversion
    }
    mem.set32(t_addr, val); //set mem to t_addr and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos) {
    int32_t rs1 = get_rs1(insn);        //variable dec for rs1, rs2, rd, and val
    int32_t rs2 = get_rs2(insn);
    int32_t rd = get_rd(insn);    
    int32_t val=regs.get(rs1) ^ regs.get(rs2); 

    if(pos) {
        string s = render_rtype(insn, "xor");   //string xor
        *pos << setw(instruction_width) << setfill(' ') << left << s;       //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " <<  //xor decodding
             hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;   //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs to rd and val
    pc+=4;  //incr pc by 4
}

void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos) {
    int32_t imm_i= get_imm_i(insn);     //variable dec for imm_i, rd, rs1, and val
    int32_t rd= get_rd(insn);
    int32_t rs1= get_rs1(insn);
    int32_t val= regs.get(rs1)^imm_i;

    if(pos) {
        string s= render_itype_alu(insn, "xori", imm_i);    //string xori
        *pos << setw(instruction_width) << setfill(' ') << left << s;       //pos formatting
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << //decodding xori
             hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;   //hex0x32 conversion
    }
    regs.set(rd, val);  //set regs to rd and val
    pc+=4;  //incr pc by 4
}