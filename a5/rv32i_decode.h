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
#ifndef RV32I_H
#define RV32I_H
#include <string>
#include <cstdint>

class rv32i_decode : public hex
{
public:

	///@parm addr The memory address where the insn is stored.
/*****************************************************************************
 * Returns a string containing the dissassemble instruction text through various
 * switch cases
 * 
 * @param addr Calculates the PC-relative target address shown in the J and B type instructions
 * @param insn The 32 bit value that must be handled
 * 
 * @return string of the dissembled instruction in text output
 *****************************************************************************/
	static std::string decode(uint32_t addr, uint32_t insn);

protected:
	static constexpr int mnemonic_width		= 8;

	static constexpr uint32_t opcode_lui		= 0b0110111;
	static constexpr uint32_t opcode_auipc		= 0b0010111;
	static constexpr uint32_t opcode_jal		= 0b1101111;
	static constexpr uint32_t opcode_jalr		= 0b1100111;
	static constexpr uint32_t opcode_btype		= 0b1100011;
	static constexpr uint32_t opcode_load_imm	= 0b0000011;
	static constexpr uint32_t opcode_stype		= 0b0100011;
	static constexpr uint32_t opcode_alu_imm	= 0b0010011;
	static constexpr uint32_t opcode_rtype		= 0b0110011;
	static constexpr uint32_t opcode_system		= 0b1110011;

	static constexpr uint32_t funct3_beq		= 0b000;
	static constexpr uint32_t funct3_bne		= 0b001;
	static constexpr uint32_t funct3_blt		= 0b100;
	static constexpr uint32_t funct3_bge		= 0b101;
	static constexpr uint32_t funct3_bltu		= 0b110;
	static constexpr uint32_t funct3_bgeu		= 0b111;

	static constexpr uint32_t funct3_lb		= 0b000;
	static constexpr uint32_t funct3_lh		= 0b001;
	static constexpr uint32_t funct3_lw		= 0b010;
	static constexpr uint32_t funct3_lbu		= 0b100;
	static constexpr uint32_t funct3_lhu		= 0b101;

	static constexpr uint32_t funct3_sb		= 0b000;
	static constexpr uint32_t funct3_sh		= 0b001;
	static constexpr uint32_t funct3_sw		= 0b010;

	static constexpr uint32_t funct3_add		= 0b000;
	static constexpr uint32_t funct3_sll		= 0b001;
	static constexpr uint32_t funct3_slt		= 0b010;
	static constexpr uint32_t funct3_sltu		= 0b011;
	static constexpr uint32_t funct3_xor		= 0b100;
	static constexpr uint32_t funct3_srx		= 0b101;
	static constexpr uint32_t funct3_or		= 0b110;
	static constexpr uint32_t funct3_and		= 0b111;

	static constexpr uint32_t funct7_srl		= 0b0000000;
	static constexpr uint32_t funct7_sra		= 0b0100000;

	static constexpr uint32_t funct7_add		= 0b0000000;
	static constexpr uint32_t funct7_sub		= 0b0100000;

	static constexpr uint32_t insn_ecall		= 0x00000073;
	static constexpr uint32_t insn_ebreak		= 0x00100073;

	static constexpr uint32_t funct3_csrrw		= 0b001;
	static constexpr uint32_t funct3_csrrs		= 0b010;
	static constexpr uint32_t funct3_csrrc		= 0b011;
	static constexpr uint32_t funct3_csrrwi		= 0b101;
	static constexpr uint32_t funct3_csrrsi		= 0b110;
	static constexpr uint32_t funct3_csrrci		= 0b111;

/*****************************************************************************
 * Returns and extracts the opcode field from a given instruction
 * 
 * @param insn The instruction to get the op code
 * 
 * @return The opcode 
 *****************************************************************************/
	static uint32_t get_opcode(uint32_t insn);

/*****************************************************************************
 * Extract and return the rd field from the given instruction
 * as an integer value from 0-31 
 * 
 * @param insn instruction to get the op code
 * 
 * @return The rd field
 *****************************************************************************/	
	static uint32_t get_rd(uint32_t insn);

/*****************************************************************************
 * Extracts and returns the funct3 field from given instruction as int value 0-7
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as value 0-7
 *****************************************************************************/	
	static uint32_t get_funct3(uint32_t insn);

/*****************************************************************************
 * Extracts and returns rs1 from given instruction 0-31
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as value 0-31
 *****************************************************************************/	
	static uint32_t get_rs1(uint32_t insn);

/*****************************************************************************
 * Extract and return rs2 from given instrction as int value 0-31
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as value 0-31
 *****************************************************************************/	
	static uint32_t get_rs2(uint32_t insn);

/*****************************************************************************
 * Extract and return funct7 from given instruction as integer 
 * from 0x00 to 0x7f
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as value 0x00 to 0x7f
 *****************************************************************************/	
	static uint32_t get_funct7(uint32_t insn);

/*****************************************************************************
 * Extract and return get_imm_i from given instruction as 32 bit integer
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as 32 bit int value
 *****************************************************************************/	
	static int32_t get_imm_i(uint32_t insn);

/*****************************************************************************
 * Extract and return get_imm_u from given instruction as 32 bit integer 
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as 32 bit int value
 *****************************************************************************/	
	static int32_t get_imm_u(uint32_t insn);

/*****************************************************************************
 * Extract and return get_imm_b from given instruction as a 32 bit int
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as a 32 bit int value
 *****************************************************************************/	
	static int32_t get_imm_b(uint32_t insn);

/*****************************************************************************
 * Extract and return the imm_s field from the given instruction
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as imm_s field
 *****************************************************************************/	
	static int32_t get_imm_s(uint32_t insn);

/*****************************************************************************
 * Extract and return imm_j field from the given instruction as a 32 bit int
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as imm_j field
 *****************************************************************************/	
	static int32_t get_imm_j(uint32_t insn);

	static constexpr uint32_t XLEN = 32;

/*****************************************************************************
 * Renders invalid instructions 
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as a void
 *****************************************************************************/
	static std::string render_illegal_insn(uint32_t insn);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the lui
 * 
 * @param insn instruction to get the op code
 * 
 * @return insn as lui field
 *****************************************************************************/
	static std::string render_lui(uint32_t insn);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the auipc
 * 
 * @param insn instruction to get the op code
 * 
 * @return insn as auipc field
 *****************************************************************************/	
	static std::string render_auipc(uint32_t insn);

	///@parm addr The memory address where the insn is stored.
/*****************************************************************************
 * Helper function to eliminate messy replicated code for the jal
 * 
 * @param addr memory address
 * @param insn instruction to get the op code
 * 
 * @return insn as jal field
 *****************************************************************************/	
	static std::string render_jal(uint32_t addr, uint32_t insn);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the jalr
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as jalr field
 *****************************************************************************/
	static std::string render_jalr(uint32_t insn);

	///@parm addr The memory address where the insn is stored.
/*****************************************************************************
 * Helper function to eliminate messy replicated code for the btype
 * 
 * @param addr memory address
 * @param insn instruction to get the op code 
 * @param mnemonic The instruction mnemonic
 * 
 * @return insn as btype
 *****************************************************************************/	
	static std::string render_btype(uint32_t addr, uint32_t insn, const char *mnemonic);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the itype_load
 * 
 * @param insn instruction to get the op code
 * @param mnemonic The instruction mnemonic
 * 
 * @return insn as a itype
 *****************************************************************************/
	static std::string render_itype_load(uint32_t insn, const char *mnemonic);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the stype
 * 
 * @param insn instruction to get the op code
 * @param mnmonic The instruction mnemonic
 * 
 * @return insn as an stype
 *****************************************************************************/	
	static std::string render_stype(uint32_t insn, const char *mnemonic);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the alu
 * 
 * @param insn instruction to get the op code
 * @param mnemonic The instruction mnemonic
 * @param imm_i The instruction imm_i code
 * 
 * @return insn as an itype alu
 *****************************************************************************/	
	static std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the rtype
 * 
 * @param insn instruction to get the op code
 * @param mnemonic The instruction mnemonic
 * 
 * @return insn as an rtype
 *****************************************************************************/	
	static std::string render_rtype(uint32_t insn, const char *mnemonic);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the ecall
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as a void
 *****************************************************************************/	
	static std::string render_ecall(uint32_t insn);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the ebreak
 * 
 * @param insn instruction to get the op code 
 * 
 * @return insn as a void
 *****************************************************************************/	
	static std::string render_ebreak(uint32_t insn);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the csrrx
 * 
 * @param insn instruction to get the op code
 * @param mnemonic The instruction mnemonic
 * 
 * @return insn as a void
 *****************************************************************************/	
	static std::string render_csrrx(uint32_t insn, const char *mnemonic);

/*****************************************************************************
 * Helper function to eliminate messy replicated code for the csrrxi
 * 
 * @param insn instruction to get the op code
 * @param mnemonic The instruction mnemonic
 * 
 * @return insn as a void
 *****************************************************************************/	
	static std::string render_csrrxi(uint32_t insn, const char *mnemonic);

/*****************************************************************************
 * Renders a string with the given number r into the register
 * 
 * @param r The given number r to be rendered into a string
 * 
 * @return The string with the value given to r
 *****************************************************************************/
	static std::string render_reg(int r);

/*****************************************************************************
 * Renders a string and the operand of the form imm(register) for instructions
 * that have addressing mode 
 * 
 * @param base  The instruction given out by the addressing mode
 * @param disp The value rendered by the string through render_reg()
 * 
 * @return prints the imm value in decimal
 *****************************************************************************/	
	static std::string render_base_disp(uint32_t base, int32_t disp);

/*****************************************************************************
 * Renders given instrcution mnemonic with proepr space padding on
 *  right side to make it mnemonic_width long
 *
 * @param m The instruction mnemonic
 * 
 * @return the os.str() in uniform width
 *****************************************************************************/	
	static std::string render_mnemonic(const std::string &m);
};

#endif