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
#ifndef HART_H
#define HART_H
#include "registerfile.h"
#include "memory.h"

class rv32i_hart : public rv32i_decode
{
public:

/****************************************************************************
 * Constructor that initializes memory
 *
 * @param m The memory to be initialized
 ***************************************************************************/
    rv32i_hart(memory &m) : mem(m) { }

/****************************************************************************
 * Mutator for show_instructions, will show each instruction executed
 *
 * @param b whether or not the instruction was executed
 ***************************************************************************/	
    void set_show_instructions(bool b) { show_instructions = b; }

/****************************************************************************
 * Mutator for show_registers, will dump registers before executed
 *
 * @param b Bool value to determine if registers will be dumped
 ***************************************************************************/	
    void set_show_registers(bool b) { show_registers = b; }

/****************************************************************************
 * Accessor for halt
 *
 * @return true if the hart has been halted for any reason
 ***************************************************************************/	
    bool is_halted() const { return halt; }

/****************************************************************************
 * Shows the reason for the hart being halted
 *
 * @return Returns a string indicating the reason for the halt
 ***************************************************************************/	
    const std::string &get_halt_reason() const { return halt_reason; }

/****************************************************************************
 * Accessor for the insn_counter
 *
 * Returns the number of instructions executed since the last calling
 * of the reset() function
 ***************************************************************************/	
    uint64_t get_insn_counter() const { return insn_counter; }

/****************************************************************************
 * Mutator for mhartid
 *
 * @param i value of the ID to be returned by csrrs instructions
 ***************************************************************************/	
    void set_mhartid(int i) { mhartid = i; }

/****************************************************************************
 * Tells the simulator how to execute and instruction
 *
 * @param hdr will be printed on the left of any and all output displayed
 ***************************************************************************/
    void tick(const std::string &hdr="");

/****************************************************************************
 * Dumps the entire state of the hart
 *
 * @param hdr default string ensuring prefix isnt printed
 ***************************************************************************/
    void dump(const std::string &hdr="") const;

/****************************************************************************
 * Reset the rv32i object and the registerfile
 ***************************************************************************/	
    void reset();

private:

    static constexpr int instruction_width = 35;

/****************************************************************************
 * Executes given RV32I instruction through the get_xxx() methods to
 * extract needed instruction fields and decodes them using switch-logic
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/
    void exec(uint32_t insn, std::ostream*);

/****************************************************************************
 * Used as a simulation of illegal instruction attempting to be executed
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
    void exec_illegal_insn(uint32_t insn, std::ostream*);

/****************************************************************************
 * Used as a simulation of an add instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/
    void exec_add(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of addi instruction 
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
    void exec_addi(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of an and instruction 
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
    void exec_and(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of an andi instruction 
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_andi(uint32_t insn, std::ostream* pos);    

/****************************************************************************
 * Used as a simulation of an auipc instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
    void exec_auipc(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a beq instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_beq(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a bge instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_bge(uint32_t insn, std::ostream* pos);  

/****************************************************************************
 * Used as a simulation of a bgeu instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_bgeu(uint32_t insn, std::ostream* pos); 

/****************************************************************************
 * Used as a simulation of a blt (BaconLetuceTomato) instruction (Yummy)
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_blt(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a bltu instruction 
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_bltu(uint32_t insn, std::ostream* pos);            

/****************************************************************************
 * Used as a simulation of a bne instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
    void exec_bne(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of an csrrs instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_csrrs(uint32_t insn, std::ostream* pos);    

/****************************************************************************
 * Used as a simulation of an ebreak instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_ebreak(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a jal instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_jal(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a jalr instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_jalr(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a lb instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/
	void exec_lb(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a lbu instruction
 *
 * Ever since I could remember...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_lbu(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a lh instruction
 *
 * There before my eyes...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_lh(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a lhu instruction
 *
 * Stood those miserable walls...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_lhu(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a lui instruction
 *
 * Lands of firey water...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
   	void exec_lui(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a lw instruction
 *
 * Plains of frozen ice...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_lw(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of an or instruction
 *
 * Endless fields of snowy sand...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
    void exec_or(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of an ori instruction 
 *
 * The outside world is so much bigger than I ever could have imagined!
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
    void exec_ori(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a sb instruction 
 *
 * Those that would reach it... would be the freest of all.
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_sb(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a sh instruction
 *
 * This is...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_sh(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a sll instruction
 *
 * Freedom...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_sll(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a slli instruction
 *
 * 		*Orechestral music plays*
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/			
	void exec_slli(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a slt instruction
 *
 * I've finally reached this sight!
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_slt(uint32_t insn, std::ostream* pos);	

/****************************************************************************
 * Used as a simulation of a slti instruction
 *
 * Hey! Armin!
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/				
	void exec_slti(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a sltu instruction
 *
 * 			...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_sltu(uint32_t insn, std::ostream* pos);	

/****************************************************************************
 * Used as a simulation of a sltiu instruction
 *
 * 		E- Eren...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/		
	void exec_sltiu(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a sra instruction
 *
 *  To be continued...
 * 
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_sra(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a srai instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_srai(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a srl instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_srl(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a srli instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/			
	void exec_srli(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a sub instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_sub(uint32_t insn, std::ostream* pos);	

/****************************************************************************
 * Used as a simulation of a sw instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/		
	void exec_sw(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a xor instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_xor(uint32_t insn, std::ostream* pos);

/****************************************************************************
 * Used as a simulation of a xori instruction
 *
 * @param insn Instruction that will be executed
 * @param pos Pointer to output object 
 ***************************************************************************/	
	void exec_xori(uint32_t insn, std::ostream* pos);    
    
    bool halt = { false };
    std::string halt_reason = { "none" };

    uint64_t insn_counter = { 0 };
    uint32_t pc = { 0 };
    uint32_t mhartid = { 0 };

    bool show_instructions = false;
    bool show_registers = false;

protected :
    registerfile regs;
    memory &mem;
};

#endif